#include "memory.h"

void *memoryset(void *ptr, int c, size_t size)
{
  char *c_ptr = (char *)ptr;

  for (int i = 0; i < size; i++)
  {
    c_ptr[i] = (char)c;
  }
  return ptr;
}
