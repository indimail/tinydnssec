#include "substdio.h"
#include "subfd.h"
#include <unistd.h>
#include "strerr.h"
#include "ip6.h"
#include "dns.h"

#define FATAL "dnsipq: fatal: "

static char seed[128];

static stralloc in;
static stralloc fqdn;
static stralloc out;
char str[IP6_FMT];

int main(int argc,char **argv)
{
  int i;

  dns_random_init(seed);

  if (*argv) ++argv;

  while (*argv) {
    if (!stralloc_copys(&in,*argv))
      strerr_die2x(111,FATAL,"out of memory");
    if (dns_ip6_qualify(&out,&fqdn,&in) == -1)
      strerr_die4sys(111,FATAL,"unable to find IP6 address for ",*argv,": ");

    substdio_put(subfdout,fqdn.s,fqdn.len);
    substdio_puts(subfdout," ");
    for (i = 0;i + 16 <= out.len;i += 16) {
      substdio_put(subfdout,str,ip6_fmt(str,out.s + i));
      substdio_puts(subfdout," ");
    }
    substdio_puts(subfdout,"\n");

    ++argv;
  }

  substdio_flush(subfdout);
  _exit(0);
}
