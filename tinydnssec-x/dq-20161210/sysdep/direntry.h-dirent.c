#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main(int argc,char **argv){

  printf("/* Public domain. */\n\n");
  printf("#ifndef _DIRENTRY_H____\n");
  printf("#define _DIRENTRY_H____\n\n");
  printf("#include <sys/types.h>\n");
  printf("#include <dirent.h>\n\n");
  printf("#define direntry struct dirent\n\n");
  printf("#endif /* _DIRENTRY_H____ */\n");
  return 0;
}
