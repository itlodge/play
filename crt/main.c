#include "stdio.h"
#include "string.h"
#include "heap.h"

int
main(int argc, char *argv[])
{
  char *buf = (char *)malloc(10);
  *buf = 'a';
  *(buf + 1) = 'b';
  *(buf + 2) = 'c';
  *(buf + 3) = '\0';

  if (strequal(buf, "abc")) {
    printf("Equal\n");
  }
  printf("%d\n", (int)strlen(buf));
  printf("%s\n", buf);

  free(buf);

  return 0;
}
