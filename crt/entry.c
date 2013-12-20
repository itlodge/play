#include "heap.h"
#include "string.h"
#include "stdio.h"

extern int
main(int argc, char *argv[]);

static void
exit(int exit_code);

static void
my_crt_fatal_error(const char *str);

void
my_crt_entry()
{
  char *ebp_reg = NULL;

  asm("movl %%ebp, %0 \n\t"
      : "=r"(ebp_reg));

  int argc = *(int *)(ebp_reg + 4);
  char **argv = (char **)(ebp_reg + 8);

  if (!heap_init()) {
    my_crt_fatal_error("Failed to intialize heap");
  }
  exit(main(argc, argv));
}

static void
my_crt_fatal_error(const char *str)
{
  printf("%s\n", str);
  exit(1);
}

static void
exit(int exit_code)
{
  asm("movl %0, %%ebx \n\t"
      "movl $1, %%eax \n\t"
      "int $0x80 \n\t"
      "hlt \n\t"
      :
      : "m"(exit_code));
}

