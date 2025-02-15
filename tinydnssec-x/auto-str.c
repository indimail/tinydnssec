#include "substdio.h"
#include <unistd.h>

char bspace[256];
substdio b = SUBSTDIO_FDBUF((ssize_t (*)(int,  char *, size_t)) write,1,bspace,sizeof bspace);

void myputs(const char *s)
{
  if (substdio_puts(&b,s) == -1) _exit(111);
}

int main(int argc,char **argv)
{
  char *name;
  char *value;
  unsigned char ch;
  char octal[4];

  name = argv[1];
  if (!name) _exit(100);
  value = argv[2];
  if (!value) _exit(100);

  myputs("const char ");
  myputs(name);
  myputs("[] = \"\\\n");

  while ((ch = *value++)) {
    myputs("\\");
    octal[3] = 0;
    octal[2] = '0' + (ch & 7); ch >>= 3;
    octal[1] = '0' + (ch & 7); ch >>= 3;
    octal[0] = '0' + (ch & 7);
    myputs(octal);
  }

  myputs("\\\n\";\n");
  if (substdio_flush(&b) == -1) _exit(111);
  _exit(0);
}
