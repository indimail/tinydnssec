#include "substdio.h"
#include "subfd.h"
#include <unistd.h>
#include "strerr.h"
#include "dns.h"

#define FATAL "dnstxt: fatal: "

static char seed[128];

static stralloc fqdn;
static stralloc out;

int main(int argc,char **argv)
{
  dns_random_init(seed);

  if (*argv) ++argv;

  while (*argv) {
    if (!stralloc_copys(&fqdn,*argv))
      strerr_die2x(111,FATAL,"out of memory");
    if (dns_txt(&out,&fqdn) == -1)
      strerr_die4sys(111,FATAL,"unable to find TXT records for ",*argv,": ");

    substdio_put(subfdout,out.s,out.len);
    substdio_puts(subfdout,"\n");

    ++argv;
  }

  substdio_flush(subfdout);
  _exit(0);
}
