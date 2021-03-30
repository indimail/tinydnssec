/*
 * Usage:  dns-notify ...
 *
 *  This programs notifies DNS slave servers to check for updates
 *  of zone information.  I wrote this to help with interoperation
 *  with our tinydns server and our BIND slave servers.  There are
 *  some Perl utilities that do the same thing, but I didn't have
 *  perl installed, and it seemed easier to write this than to install
 *  PERL + Net::DNS + patch to get it working.  Besides, if C is good
 *  enough for djbdns, ...
 *
 * Written by
 *
 *  Joseph Tam <tam (at) math (dot) ubc (dot) ca>
 *  Department of Mathematics
 *  University of British Columbia
 *  Canada V6T 1Z2
 *
 *  Last updated: 2011-01-12
 *
 * Permission is freely given to use, distribute or mangle.
 *
 */
#include <unistd.h>
#include <stdlib.h>
#include <sgetopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h>
#include <setjmp.h>
#include <str.h>
#include <byte.h>
#include <strerr.h>
#include <substdio.h>
#include <subfd.h>
#include <scan.h>
#include <fmt.h>
#include <stralloc.h>
#include "dns-notify.h"

#define FATAL "dns-notify: fatal: "

int             s = 0,			/*- Socket handle */
                nretries = 3,	/*- Number of times to retry */
                try,			/*- Current number of attempts */
                timeout = 2,	/*- Number of seconds to wait for response */
                verbose = 0;	/*- Toggle verbose flag: default = no */
sigjmp_buf      env;            /*- Jump buffer */
static stralloc t;
char            strnum[FMT_ULONG];


const char *usage = 
	"Usage: dns-notify [-h | -s source | -d dest | -r retries | -t timeout | -v | zone...] ...\n"
	"            -h This help message\n"
	"            -s ip Source IP of notify messages\n"
	"               useful if host is multi-homed and is not zone serving\n"
	"               from its primary interface. [default=host's primary IP]\n"
	"            -d ip IP or hostname to send notify messages to. [default=none]\n"
	"            -r int Maximum retries ofnotify request. [default=3]\n"
	"            -t int Timeout (in seconds) for responses before retry. [default=2]\n"
	"            -v Toggle verbose diagnostic flag. [default=quiet]\n"
	"            zone... Zone(s) to update.\n\n"
	" Arguments are acted upon in the order they are given. Multiple zones\n"
	" on different slave servers can be notified:\n"
	"     -dslave1 zone1 -dslave2 zone2 ...\n\n"
	" If a set of zones are replicated by multiple slave servers, you'll have\n"
	" to repeat the set for each server:\n\n"
	"     -dslave1 zones ... -dslave2 zones ...";

void
out(const char *str)
{
	if (!str || !*str)
		return;
	if (substdio_puts(subfdout, str) == -1)
		strerr_die2sys(111, FATAL, "write: ");
	return;
}

void
flush()
{
	if (substdio_flush(subfdout) == -1)
		strerr_die2sys(111, FATAL, "write: ");
}

/* Extract rcode from reply packet and return string message. */
const char     *
ResponseCode(const struct DNS_header *h)
{
	const char     *mys;
	int             i;

	switch (h->rcode) {
	/*- Grabbed from RFC2929 */
	case 0:
		mys = "OK (No error)";
		break;
	case 1:
		mys = "Format error";
		break;
	case 2:
		mys = "Server failure";
		break;
	case 3:
		mys = "Non-existent domain";
		break;
	case 4:
		mys = "Not implemented";
		break;
	case 5:
		mys = "Query refused";
		break;
	case 6:
		mys = "Name exists, but shouldn't";
		break;
	case 7:
		mys = "RR Exists, but shouldn't";
		break;
	case 8:
		mys = "RR doesn't exist, but should";
		break;
	case 9:
		mys = "Server not authoratative for zone";
		break;
	case 10:
		mys = "Name not contained in zone";
		break;
	case 15:
		mys = "Bad OPT version";
		break;
	/*
	 * There are others, but you're not likely to get them ... 
	 */
	default:
		strnum[i = fmt_int(strnum, h->rcode)] = 0;
		if (!stralloc_copyb(&t, "Unknown rcode (", 15) ||
				!stralloc_catb(&t, strnum, i) ||
				!stralloc_0(&t))
			strerr_die2sys(111, FATAL, "out of memory");
		mys = t.s;
	}
	return mys;
}


/* Create network socket to send/receive DNS messages. */
void
open_socket(char *myname)
{
	struct hostent *h;			/* Host entry for self */
	struct sockaddr_in me;		/* Socket addresses of source host */

	if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		strerr_die2sys(111, FATAL, "socket: ");
	me.sin_family = AF_INET;
	if ((h = gethostbyname(myname)) == NULL)
		strerr_die4sys(111, FATAL, "gethostbyname: ", myname, ": ");
	byte_copy((char *) &me.sin_addr, h->h_length, h->h_addr);
	if (bind(s, (struct sockaddr *) &me, sizeof (me)) != 0)
		strerr_die2sys(111, FATAL, "bind: ");
}


/* Output verbose decompilation of DNS query/reply. */
void
print_dnsheader(const char *title, struct DNS_header *h)
{
	const char     *mys;
	int             i;

	out("\n\t=== ");
	out(title);
	out(" ===\n");
	out("\tQuery ID            = 0x");
	strnum[fmt_xlong(strnum, h->id)] = 0;
	out(strnum);
	out("\n");
	out("\tQuery/Response      = ");
	out(h->qr ? "response\n" : "query\n");
	switch (h->opcode)
	{
	case 0:
		mys = "Query";
		break;
	case 1:
		mys = "Inverse query";
		break;
	case 2:
		mys = "Status";
		break;
	case 4:
		mys = "Notify";
		break;
	case 5:
		mys = "Update";
		break;
	default:
		strnum[i = fmt_int(strnum, h->rcode)] = 0;
		if (!stralloc_copyb(&t, "Unknown opcode (", 16) ||
				!stralloc_catb(&t, strnum, i) ||
				!stralloc_0(&t))
			strerr_die2sys(111, FATAL, "out of memory");
		mys = t.s;
	}
	out("\tOpcode              = ");
	out(mys);
	out("\n");
	out("\tAuth. answer        = ");
	out(h->aa ? "[]" : "non-");
	out("authorative\n");
	out("\tTruncated           = ");
	out(h->trunc ? "yes" : "no");
	out("\n");
	out("\tRecursion desired   = ");
	out(h->rd ? "yes" : "no");
	out("\n");
	out("\tRecursion available = ");
	out(h->ra ? "yes" : "no");
	out("\n");
	out("\tResponse code       = ");
	out(ResponseCode(h));
	out("\n");
	out("\tQuestion count      = ");
	strnum[fmt_int(strnum, h->qdcount)] = 0;
	out(strnum);
	out("\n");
	out("\tAnswer count        = ");
	strnum[fmt_int(strnum, h->ancount)] = 0;
	out(strnum);
	out("\n");
	out("\tNameserver RR count = ");
	strnum[fmt_int(strnum, h->nscount)] = 0;
	out(strnum);
	out("\n");
	out("\tAdditional RR count = ");
	strnum[fmt_int(strnum, h->arcount)] = 0;
	out(strnum);
	out("\n");
	flush();
}


/* Create DNS NOTIFY packet. */
void
MakeNotifyPacket(const char *zone, unsigned char packet[MAXPACK], int *packetlen)
{
	struct DNS_header dh;
	struct DNS_query dq;
	unsigned        zlen;
	unsigned char  *zp;
	register int    i, k;
	char            strnum1[FMT_ULONG], strnum2[FMT_ULONG];


	zlen = str_len(zone) + 2;
	*packetlen = sizeof (dh) + zlen + sizeof (dq);

	if (*packetlen >= MAXPACK) {
		strnum1[fmt_int(strnum1, *packetlen)] = 0;
		strnum2[fmt_int(strnum2, MAXPACK)] = 0;
		strerr_die5x(100, FATAL, "Packet size is to large: ", strnum1, ">= ", strnum2);
	}

	dh.id = rand();
	dh.qr = 0;
	dh.opcode = 4;
	dh.aa = 1;
	dh.trunc = 0;
	dh.rd = 0;
	dh.ra = 0;
	dh.z = 0;
	dh.rcode = 0;
	dh.qdcount = 1;
	dh.ancount = 0;
	dh.nscount = 0;
	dh.arcount = 0;
	byte_copy((char *) packet, sizeof(dh), (char *) &dh);

	/*- Make zone labels */
	zp = &packet[sizeof (dh)];
	byte_copy((char *) &zp[1], zlen - 1, zone);
	zp[0] = '.';
	k = 0;

	while (zp[k]) {
		for (i = k + 1; zp[i] && zp[i] != '.'; i++);
		zp[k] = i - k - 1;
		k = i;
	}

	dq.qtype = 6;
	dq.qclass = 1;
	byte_copy((char *) &packet[sizeof (dh) + zlen], sizeof(dq), (char *) &dq);
}


/* Timeout handler for UDP reply. */
static void
timeout_handler(int sig)
{
	if (substdio_put(subfdout, "timeout.\n", 9) == -1)
		strerr_die2sys(111, FATAL, "write: ");
	flush();
	try++;
	siglongjmp(env, sig);
}


/* Send NOTIFY packet and wait for response. */
void
SendPacket(const char *slave_name, unsigned char packet[MAXPACK], const int packetlen)
{
	struct hostent *h;
	struct sockaddr_in slave_addr;	/* Socket addresses of slave server */
	unsigned char   response[MAXPACK];
	int             i, resp_len, slen;

	slave_addr.sin_family = AF_INET;
	slave_addr.sin_port = DOMAIN_PORT;
	if ((h = gethostbyname(slave_name)) == NULL)
		strerr_die4sys(111, FATAL, "gethostbyname: ", slave_name, ": ");
	byte_copy((char *) &slave_addr.sin_addr, h->h_length, h->h_addr);
	if (verbose)
		print_dnsheader("Send", (struct DNS_header *) packet);
	try = 1;
	sigsetjmp(env, 1);
	signal(SIGALRM, timeout_handler);
	alarm(timeout);

	if (try <= nretries) {
		strnum[i = fmt_int(strnum, try)] = 0;
		if (substdio_put(subfdout, "\ttry#", 5) == -1 ||
				substdio_put(subfdout, strnum, i) == -1)
			strerr_die2sys(111, FATAL, "write: ");
		if (sendto(s, packet, packetlen, 0, (struct sockaddr *) &slave_addr, sizeof (slave_addr)) < 0)
			strerr_die2sys(111, FATAL, "sendto: ");
		if (substdio_put(subfdout, "sent, ", 6) == -1)
			strerr_die2sys(111, FATAL, "write: ");

		/*-
		 * When a NOTIFY message is sent, a response should be received which is
		 * is the same as the query, but with the qr flag set.
		 */
		slen = sizeof(slave_addr);
		if ((resp_len = recvfrom(s, response, MAXPACK, 0, (struct sockaddr *) &slave_addr, (socklen_t *) & slen)) < 0)
			strerr_die2sys(111, FATAL, "recvfrom: ");
		if (verbose)
			print_dnsheader("Reply", (struct DNS_header *) &response);
		else 
		if (substdio_put(subfdout, "reply [", 8) == -1 ||
				substdio_puts(subfdout, ResponseCode((const struct DNS_header *) &response)) == -1 ||
				substdio_put(subfdout, "\n", 1) == -1)
			strerr_die2sys(111, FATAL, "write: ");
	} else
	if (substdio_put(subfdout, "\tGive up.\n", 10) == -1)
		strerr_die2sys(111, FATAL, "write: ");
	flush();
}

int
main(int argc, char *argv[])
{
	unsigned char   packet[MAXPACK];
	int             packetlen;
	int             opt;
	char           *dest, *source;

	if (argc == 1)
		strerr_die1x(100, usage);

	/*- Seed PRNG to get query IDs */
	srand(getpid());
	source = dest = (char *) 0;
	while ((opt = getopt(argc, argv,"hvd:r:s:t:")) != opteof) {
		switch(opt)
		{
		case 'v':
			verbose = !verbose;
			break;
		case 'd':
			dest = optarg;
			break;
		case 'r':
			scan_int(optarg, &nretries);
			break;
		case 's':
			source = optarg;
			break;
		case 't':
			scan_int(optarg, &timeout);
			break;
		case 'h':
		default:
			strerr_die1x(100, usage);
			break;
		}
	}
	if (source)
		open_socket(source);
	if (!dest) 
		strerr_die1x(100, "No destination set");
	while (optind < argc) {
		strerr_warn7("Zone=", argv[optind], " (", source, "=>", dest, ")\n", 0);
		MakeNotifyPacket(argv[optind++], packet, &packetlen);
		SendPacket(dest, packet, packetlen);
	}
}
