#include <stdio.h>

main()
{
  char string[1024];
  sprintf(string,"echo %s | %s > %s\n","NOP", "/home/neat/bin/tcs_talk", "/home/neat/tcs.status");
  fprintf(stderr,"string is %s",string);
  system(string);
  exit(0);
}
