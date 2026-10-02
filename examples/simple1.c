#include <stdio.h>
#include <csptr/smart_ptr.h>

int main(void) {
  // some_int is an unique_ptr to an int.
  // It doesn't have a specific destructor set.
  int *some_int = unique_ptr(sizeof(int));
  *some_int = 1;

  printf("%p = %d", some_int, *some_int);

  sfree(some_int);
  return 0;
}
