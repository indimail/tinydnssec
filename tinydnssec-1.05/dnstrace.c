#include "uint16.h"
#include "uint32.h"
#include "fmt.h"
#include "str.h"
#include "byte.h"
#include "ip4.h"
#include "ip6.h"
#include "gen_alloc.h"
#include "gen_allocdefs.h"
#include <unistd.h>
#include "substdio.h"
#include "subfd.h"
#include "stralloc.h"
#include "error.h"
#include "strerr.h"
#include "iopause.h"
#include "printrecord.h"
#include "alloc.h"
#include "parsetype.h"
#include "dd.h"
#include "dns.h"

#define FATAL "dnstrace: fatal: "

void
nomem(void)
{
	strerr_die2x(111, FATAL, "out of memory");
}

void
usage(void)
{
	strerr_die1x(100, "dnstrace: usage: dnstrace type name rootip ...");
}

static stralloc querystr;
char            ipstr[IP6_FMT];
static stralloc tmp;

void
printdomain(const char *d)
{
	if (!stralloc_copys(&tmp, ""))
		nomem();
	if (!dns_domain_todot_cat(&tmp, d))
		nomem();
	substdio_put(subfdout, tmp.s, tmp.len);
}

static struct dns_transmit tx;

int
resolve(char *q, char qtype[2], char ip[16])
{
	struct taia     start;
	struct taia     stamp;
	struct taia     deadline;
	char            servers[256];
	iopause_fd      x[1];
	int             r;

	taia_now(&start);

	byte_zero(servers, 256);
	byte_copy(servers, 16, ip);

	if (dns_transmit_start(&tx, servers, 0, q, qtype, (const char *) V6any) == -1)
		return -1;

	for (;;) {
		taia_now(&stamp);
		taia_uint(&deadline, 120);
		taia_add(&deadline, &deadline, &stamp);
		dns_transmit_io(&tx, x, &deadline);
		iopause(x, 1, &deadline, &stamp);
		r = dns_transmit_get(&tx, x, &stamp);
		if (r == -1)
			return -1;
		if (r == 1)
			break;
	}

	taia_now(&stamp);
	taia_sub(&stamp, &stamp, &start);
	taia_uint(&deadline, 1);
	if (taia_less(&deadline, &stamp)) {
		substdio_put(subfdout, querystr.s, querystr.len);
		substdio_puts(subfdout, "ALERT:took more than 1 second\n");
	}

	return 0;
}

struct address {
	char           *owner;
	char            ip[16];
};

GEN_ALLOC_typedef(address_alloc, struct address, s, len, a)
GEN_ALLOC_readyplus(address_alloc, struct address, s, len, a, 30, address_alloc_readyplus)
GEN_ALLOC_append(address_alloc, struct address, s, len, a, 30, address_alloc_readyplus, address_alloc_append)
	static address_alloc address;

	struct ns {
		char           *owner;
		char           *ns;
	};

GEN_ALLOC_typedef(ns_alloc, struct ns, s, len, a)
GEN_ALLOC_readyplus(ns_alloc, struct ns, s, len, a, 30, ns_alloc_readyplus)
GEN_ALLOC_append(ns_alloc, struct ns, s, len, a, 30, ns_alloc_readyplus, ns_alloc_append)
	static ns_alloc ns;

	struct query {
		char           *owner;
		char            type[2];
	};

GEN_ALLOC_typedef(query_alloc, struct query, s, len, a)
GEN_ALLOC_readyplus(query_alloc, struct query, s, len, a, 30, query_alloc_readyplus)
GEN_ALLOC_append(query_alloc, struct query, s, len, a, 30, query_alloc_readyplus, query_alloc_append)
	static query_alloc query;

	struct qt {
		char           *owner;
		char            type[2];
		char           *control;
		char            ip[16];
	};

GEN_ALLOC_typedef(qt_alloc, struct qt, s, len, a)
GEN_ALLOC_readyplus(qt_alloc, struct qt, s, len, a, 30, qt_alloc_readyplus)
GEN_ALLOC_append(qt_alloc, struct qt, s, len, a, 30, qt_alloc_readyplus, qt_alloc_append)
	static qt_alloc qt;

	void            qt_add(const char *q, const char type[2], const char *control, const char ip[16])
{
	struct qt       x;
	int             i;

	if (!*q)
		return;					/* don't ask the roots about our artificial . host */

	for (i = 0; i < qt.len; ++i)
		if (dns_domain_equal(qt.s[i].owner, q))
			if (dns_domain_equal(qt.s[i].control, control))
				if (byte_equal(qt.s[i].type, 2, type))
					if (byte_equal(qt.s[i].ip, 16, ip))
						return;

	byte_zero((char *) &x, sizeof x);
	if (!dns_domain_copy(&x.owner, q))
		nomem();
	if (!dns_domain_copy(&x.control, control))
		nomem();
	byte_copy(x.type, 2, type);
	byte_copy(x.ip, 16, ip);
	if (!qt_alloc_append(&qt, &x))
		nomem();
}

void
query_add(const char *owner, const char type[2])
{
	struct query    x;
	int             i;
	int             j;

	for (i = 0; i < query.len; ++i)
		if (dns_domain_equal(query.s[i].owner, owner))
			if (byte_equal(query.s[i].type, 2, type))
				return;

	byte_zero((char *) &x, sizeof x);
	if (!dns_domain_copy(&x.owner, owner))
		nomem();
	byte_copy(x.type, 2, type);
	if (!query_alloc_append(&query, &x))
		nomem();

	for (i = 0; i < ns.len; ++i)
		if (dns_domain_suffix(owner, ns.s[i].owner))
			for (j = 0; j < address.len; ++j)
				if (dns_domain_equal(ns.s[i].ns, address.s[j].owner))
					qt_add(owner, type, ns.s[i].owner, address.s[j].ip);
}

void
ns_add(const char *owner, const char *server)
{
	struct ns       x;
	int             i;
	int             j;

	substdio_put(subfdout, querystr.s, querystr.len);
	substdio_puts(subfdout, "NS:");
	printdomain(owner);
	substdio_puts(subfdout, ":");
	printdomain(server);
	substdio_puts(subfdout, "\n");

	for (i = 0; i < ns.len; ++i)
		if (dns_domain_equal(ns.s[i].owner, owner))
			if (dns_domain_equal(ns.s[i].ns, server))
				return;

	query_add(server, DNS_T_A);

	byte_zero((char *) &x, sizeof x);
	if (!dns_domain_copy(&x.owner, owner))
		nomem();
	if (!dns_domain_copy(&x.ns, server))
		nomem();
	if (!ns_alloc_append(&ns, &x))
		nomem();

	for (i = 0; i < query.len; ++i)
		if (dns_domain_suffix(query.s[i].owner, owner))
			for (j = 0; j < address.len; ++j)
				if (dns_domain_equal(server, address.s[j].owner))
					qt_add(query.s[i].owner, query.s[i].type, owner, address.s[j].ip);
}

void
address_add(const char *owner, const char ip[16])
{
	struct address  x;
	int             i;
	int             j;

	substdio_put(subfdout, querystr.s, querystr.len);
	substdio_puts(subfdout, "A:");
	printdomain(owner);
	substdio_puts(subfdout, ":");
	if (ip6_isv4mapped(ip))
		substdio_put(subfdout, ipstr, ip4_fmt(ipstr, ip + 12));
	else
		substdio_put(subfdout, ipstr, ip6_fmt(ipstr, ip));
	substdio_puts(subfdout, "\n");

	for (i = 0; i < address.len; ++i)
		if (dns_domain_equal(address.s[i].owner, owner))
			if (byte_equal(address.s[i].ip, 16, ip))
				return;

	byte_zero((char *) &x, sizeof x);
	if (!dns_domain_copy(&x.owner, owner))
		nomem();
	byte_copy(x.ip, 16, ip);
	if (!address_alloc_append(&address, &x))
		nomem();

	for (i = 0; i < ns.len; ++i)
		if (dns_domain_equal(ns.s[i].ns, owner))
			for (j = 0; j < query.len; ++j)
				if (dns_domain_suffix(query.s[j].owner, ns.s[i].owner))
					qt_add(query.s[j].owner, query.s[j].type, ns.s[i].owner, ip);
}

char            seed[128];

static char    *t1;
static char    *t2;
static char    *referral;
static char    *cname;

static int
typematch(const char rtype[2], const char qtype[2])
{
	return byte_equal(qtype, 2, rtype) || byte_equal(qtype, 2, DNS_T_ANY);
}

void
parsepacket(const char *buf, unsigned int len, const char *d, const char dtype[2], const char *control)
{
	char            misc[20];
	char            header[12];
	unsigned int    pos;
	uint16          numanswers;
	unsigned int    posanswers;
	uint16          numauthority;
	uint16          numglue;
	uint16          datalen;
	unsigned int    rcode;
	int             flagout;
	int             flagcname;
	int             flagreferral;
	int             flagsoa;
	int             j;
	const char     *x;

	pos = dns_packet_copy(buf, len, 0, header, 12);
	if (!pos)
		goto DIE;
	pos = dns_packet_skipname(buf, len, pos);
	if (!pos)
		goto DIE;
	pos += 4;

	uint16_unpack_big(header + 6, &numanswers);
	uint16_unpack_big(header + 8, &numauthority);
	uint16_unpack_big(header + 10, &numglue);

	rcode = header[3] & 15;
	if (rcode && (rcode != 3)) {
		errno = error_proto;
		goto DIE;
	}							/* impossible */

	flagout = 0;
	flagcname = 0;
	flagreferral = 0;
	flagsoa = 0;
	posanswers = pos;
	for (j = 0; j < numanswers; ++j) {
		pos = dns_packet_getname(buf, len, pos, &t1);
		if (!pos)
			goto DIE;
		pos = dns_packet_copy(buf, len, pos, header, 10);
		if (!pos)
			goto DIE;
		if (dns_domain_equal(t1, d)) {
			if (byte_equal(header + 2, 2, DNS_C_IN)) {
				if (typematch(header, dtype))
					flagout = 1;
				else
				if (typematch(header, DNS_T_CNAME)) {
					if (!dns_packet_getname(buf, len, pos, &cname))
						goto DIE;
					flagcname = 1;
				}
			}
		}
		uint16_unpack_big(header + 8, &datalen);
		pos += datalen;
	}
	for (j = 0; j < numauthority; ++j) {
		pos = dns_packet_getname(buf, len, pos, &t1);
		if (!pos)
			goto DIE;
		pos = dns_packet_copy(buf, len, pos, header, 10);
		if (!pos)
			goto DIE;
		if (typematch(header, DNS_T_SOA))
			flagsoa = 1;
		else if (typematch(header, DNS_T_NS)) {
			flagreferral = 1;
			if (!dns_domain_copy(&referral, t1))
				goto DIE;
		}
		uint16_unpack_big(header + 8, &datalen);
		pos += datalen;
	}

	if (!flagcname && !rcode && !flagout && flagreferral && !flagsoa)
		if (dns_domain_equal(referral, control) || !dns_domain_suffix(referral, control)) {
			substdio_put(subfdout, querystr.s, querystr.len);
			substdio_puts(subfdout, "ALERT:lame server; refers to ");
			printdomain(referral);
			substdio_puts(subfdout, "\n");
			return;
		}

	pos = posanswers;
	for (j = 0; j < numanswers + numauthority + numglue; ++j) {
		pos = dns_packet_getname(buf, len, pos, &t1);
		if (!pos)
			goto DIE;
		pos = dns_packet_copy(buf, len, pos, header, 10);
		if (!pos)
			goto DIE;
		uint16_unpack_big(header + 8, &datalen);
		if (dns_domain_suffix(t1, control))
			if (byte_equal(header + 2, 2, DNS_C_IN)) {
				if (typematch(header, DNS_T_NS)) {
					if (!dns_packet_getname(buf, len, pos, &t2))
						goto DIE;
					ns_add(t1, t2);
				} else if (typematch(header, DNS_T_A) && datalen == 4) {
					if (!dns_packet_copy(buf, len, pos, misc + 12, 4))
						goto DIE;
					byte_copy(misc, 12, (const char *) V4mappedprefix);
					address_add(t1, misc);
				} else if (typematch(header, DNS_T_AAAA) && datalen == 16) {
					if (!dns_packet_copy(buf, len, pos, misc, 16))
						goto DIE;
					address_add(t1, misc);
				}
			}
		pos += datalen;
	}


	if (flagcname) {
		query_add(cname, dtype);
		substdio_put(subfdout, querystr.s, querystr.len);
		substdio_puts(subfdout, "CNAME:");
		printdomain(cname);
		substdio_puts(subfdout, "\n");
		return;
	}
	if (rcode == 3) {
		substdio_put(subfdout, querystr.s, querystr.len);
		substdio_puts(subfdout, "NXDOMAIN\n");
		return;
	}
	if (flagout || flagsoa || !flagreferral) {
		if (!flagout) {
			substdio_put(subfdout, querystr.s, querystr.len);
			substdio_puts(subfdout, "NODATA\n");
			return;
		}
		pos = posanswers;
		for (j = 0; j < numanswers + numauthority + numglue; ++j) {
			pos = printrecord(&tmp, buf, len, pos, d, dtype);
			if (!pos)
				goto DIE;
			if (tmp.len) {
				substdio_put(subfdout, querystr.s, querystr.len);
				substdio_puts(subfdout, "answer:");
				substdio_put(subfdout, tmp.s, tmp.len);	/* includes \n */
			}
		}
		return;
	}

	if (!dns_domain_suffix(d, referral))
		goto DIE;
	substdio_put(subfdout, querystr.s, querystr.len);
	substdio_puts(subfdout, "see:");
	printdomain(referral);
	substdio_puts(subfdout, "\n");
	return;

  DIE:
	x = error_str(errno);
	substdio_put(subfdout, querystr.s, querystr.len);
	substdio_puts(subfdout, "ALERT:unable to parse response packet; ");
	substdio_puts(subfdout, x);
	substdio_puts(subfdout, "\n");
}

int
main(int argc, char **argv)
{
	static stralloc out;
	static stralloc fqdn;
	static stralloc udn;
	static char    *q;
	char           *control;
	char            type[2];
	char            ip[64];
	int             i;
	uint16          u16;

	dns_random_init(seed);

	if (!stralloc_copys(&querystr, "0:.:.:start:"))
		nomem();

	if (!address_alloc_readyplus(&address, 1))
		nomem();
	if (!query_alloc_readyplus(&query, 1))
		nomem();
	if (!ns_alloc_readyplus(&ns, 1))
		nomem();
	if (!qt_alloc_readyplus(&qt, 1))
		nomem();

	if (!*argv)
		usage();
	if (!*++argv)
		usage();
	if (!parsetype(*argv, type))
		usage();

	if (!*++argv)
		usage();
	if (!dns_domain_fromdot(&q, *argv, str_len(*argv)))
		nomem();

	query_add(q, type);
	ns_add("", "");

	while (*++argv) {
		if (!stralloc_copys(&udn, *argv))
			nomem();
		if (dns_ip6_qualify(&out, &fqdn, &udn) == -1)
			nomem();			/* XXX */
		for (i = 0; i + 16 <= out.len; i += 16)
			address_add("", out.s + i);
	}

	for (i = 0; i < qt.len; ++i) {
		if (!dns_domain_copy(&q, qt.s[i].owner))
			nomem();
		control = qt.s[i].control;
		if (!dns_domain_suffix(q, control))
			continue;
		byte_copy(type, 2, qt.s[i].type);
		byte_copy(ip, 16, qt.s[i].ip);

		if (!stralloc_copys(&querystr, ""))
			nomem();
		uint16_unpack_big(type, &u16);
		if (!stralloc_catulong0(&querystr, u16, 0))
			nomem();
		if (!stralloc_cats(&querystr, ":"))
			nomem();
		if (!dns_domain_todot_cat(&querystr, q))
			nomem();
		if (!stralloc_cats(&querystr, ":"))
			nomem();
		if (!dns_domain_todot_cat(&querystr, control))
			nomem();
		if (!stralloc_cats(&querystr, ":"))
			nomem();
		if (ip6_isv4mapped(ip)) {
			if (!stralloc_catb(&querystr, ipstr, ip4_fmt(ipstr, ip + 12)))
				nomem();
		} else if (!stralloc_catb(&querystr, ipstr, ip6_fmt(ipstr, ip)))
			nomem();
		if (!stralloc_cats(&querystr, ":"))
			nomem();

		substdio_put(subfdout, querystr.s, querystr.len);
		substdio_puts(subfdout, "tx\n");
		substdio_flush(subfdout);

		if (resolve(q, type, ip) == -1) {
			const char     *x = error_str(errno);
			substdio_put(subfdout, querystr.s, querystr.len);
			substdio_puts(subfdout, "ALERT:query failed; ");
			substdio_puts(subfdout, x);
			substdio_puts(subfdout, "\n");
		} else
			parsepacket(tx.packet, tx.packetlen, q, type, control);

		if (dns_domain_equal(q, "\011localhost\0")) {
			substdio_put(subfdout, querystr.s, querystr.len);
			substdio_puts(subfdout, "ALERT:some caches do not handle localhost internally\n");
			address_add(q, "\177\0\0\1");
		}
		if (dd(q, "", ip) == 4) {
			substdio_put(subfdout, querystr.s, querystr.len);
			substdio_puts(subfdout, "ALERT:some caches do not handle IP addresses internally\n");
			address_add(q, ip);
		}

		substdio_flush(subfdout);
	}

	_exit(0);
}
