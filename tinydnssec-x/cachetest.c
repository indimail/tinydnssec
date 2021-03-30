#include "substdio.h"
#include "subfd.h"
#include <unistd.h>
#include "cache.h"
#include "str.h"

int main(int argc,char **argv)
{
  int i;
  char *x;
  char *y;
  unsigned int u;
  uint32 ttl;

  if (!cache_init(200)) _exit(111);

  if (*argv) ++argv;

  while ((x = *argv++)) {
    i = str_chr(x,':');
    if (x[i])
      cache_set(x,i,x + i + 1,str_len(x) - i - 1,86400);
    else {
      y = cache_get(x,i,&u,&ttl);
      if (y)
        substdio_put(subfdout,y,u);
      substdio_puts(subfdout,"\n");
    }
  }

  substdio_flush(subfdout);
  _exit(0);
}
