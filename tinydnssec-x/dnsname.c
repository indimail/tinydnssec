#include "substdio.h"
#include "subfd.h"
#include <unistd.h>
#include "strerr.h"
#include "ip4.h"
#include "ip6.h"
#include "dns.h"

#define FATAL "dnsname: fatal: "

static char seed[128];

char ip[4];
char ip6[16];
static stralloc out;

int main(int argc,char **argv)
{
  dns_random_init(seed);

  if (*argv) ++argv;

  while (*argv) {
    if (ip6_scan(*argv,ip6)) {
      if (dns_name6(&out,ip6) == -1)
	strerr_die4sys(111,FATAL,"unable to find host name for ",*argv,": ");
    } else {
      if (!ip4_scan(*argv,ip))
	strerr_die3x(111,FATAL,"unable to parse IP address ",*argv);
      if (dns_name4(&out,ip) == -1)
	strerr_die4sys(111,FATAL,"unable to find host name for ",*argv,": ");
    }

    substdio_put(subfdout,out.s,out.len);
    substdio_puts(subfdout,"\n");

    ++argv;
  }

  substdio_flush(subfdout);
  _exit(0);
}
