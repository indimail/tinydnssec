#include "substdio.h"
#include "subfd.h"
#include <unistd.h>
#include "strerr.h"
#include "uint16.h"
#include "byte.h"
#include "str.h"
#include "fmt.h"
#include "dns.h"

#define FATAL "dnsmx: fatal: "

void nomem(void)
{
  strerr_die2x(111,FATAL,"out of memory");
}

static char seed[128];

static stralloc fqdn;
static char *q;
static stralloc out;
char strnum[FMT_ULONG];

int main(int argc,char **argv)
{
  int i;
  int j;
  uint16 pref;

  dns_random_init(seed);

  if (*argv) ++argv;

  while (*argv) {
    if (!stralloc_copys(&fqdn,*argv)) nomem();
    if (dns_mx(&out,&fqdn) == -1)
      strerr_die4sys(111,FATAL,"unable to find MX records for ",*argv,": ");

    if (!out.len) {
      if (!dns_domain_fromdot(&q,*argv,str_len(*argv))) nomem();
      if (!stralloc_copys(&out,"0 ")) nomem();
      if (!dns_domain_todot_cat(&out,q)) nomem();
      if (!stralloc_cats(&out,"\n")) nomem();
      substdio_put(subfdout,out.s,out.len);
    }
    else {
      i = 0;
      while (i + 2 < out.len) {
	j = byte_chr(out.s + i + 2,out.len - i - 2,0);
	uint16_unpack_big(out.s + i,&pref);
	substdio_put(subfdout,strnum,fmt_ulong(strnum,pref));
	substdio_puts(subfdout," ");
	substdio_put(subfdout,out.s + i + 2,j);
	substdio_puts(subfdout,"\n");
	i += j + 3;
      }
    }

    ++argv;
  }

  substdio_flush(subfdout);
  _exit(0);
}
