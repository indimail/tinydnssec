#include "substdio.h"
#include "subfd.h"
#include <unistd.h>
#include "strerr.h"
#include "ip4.h"
#include "dns.h"

#define FATAL "dnsnamex: fatal: "

static char     seed[128];

char            ip[4];
static stralloc out;

int
main(int argc, char **argv)
{
	dns_random_init(seed);

	if (*argv)
		++argv;

	while (*argv) {
		if (!ip4_scan(*argv, ip))
			strerr_die3x(111, FATAL, "unable to parse IP address ", *argv);
		if (dns_name4_multi(&out, ip) == -1)
			strerr_die4sys(111, FATAL, "unable to find host name for ", *argv, ": ");

		substdio_put(subfdout, out.s, out.len);
		substdio_puts(subfdout, "\n");

		++argv;
	}

	substdio_flush(subfdout);
	_exit(0);
}
