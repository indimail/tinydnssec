#include "substdio.h"
#include "subfd.h"
#include <unistd.h>
#include "strerr.h"
#include "ip4.h"
#include "dns.h"

#define FATAL "dnsipq: fatal: "

static char     seed[128];

static stralloc in;
static stralloc fqdn;
static stralloc out;
char            str[IP4_FMT];

int
main(int argc, char **argv)
{
	int             i;

	dns_random_init(seed);
	if (*argv)
		++argv;
	while (*argv) {
		if (!stralloc_copys(&in, *argv))
			strerr_die2x(111, FATAL, "out of memory");
		if (dns_ip4_qualify(&out, &fqdn, &in) == -1)
			strerr_die4sys(111, FATAL, "unable to find IP address for ", *argv, ": ");
		if (substdio_put(subfdout, fqdn.s, fqdn.len) == -1 ||
				substdio_put(subfdout, " ", 1) == -1)
			_exit(111);
		for (i = 0; i + 4 <= out.len; i += 4) {
			if (substdio_put(subfdout, str, ip4_fmt(str, out.s + i)) == -1 ||
					substdio_put(subfdout, " ", 1) == -1)
				_exit(111);
		}
		if (substdio_puts(subfdout, "\n") == -1)
			_exit(111);
		++argv;
	}
	if (substdio_flush(subfdout) == -1)
		_exit(111);
	_exit(0);
}
