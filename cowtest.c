// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "types.h"
#include "user.h"
#include "stat.h"

void
cowtest(void)
{
  char* test = (char*)malloc(4096);

  printf(1, "before fork : %d\n", get_numfreepages());
  int pid = fork();

  if(pid == 0)
  {
    printf(1, "child : %d\n", get_numfreepages());
    test[0] = 'c';
    printf(1, "child : %d\n", get_numfreepages());
    exit();
  }
  else
  {
    printf(1, "parent : %d\n", get_numfreepages());
    wait();
    printf(1, "after child exit : %d\n", get_numfreepages());
    exit();
  }
  printf(1, "%d\n", *test);
}

int
main(void)
{
  cowtest();
  exit();
}
