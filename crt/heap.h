#ifndef __HEAP_H__
#define __HEAP_H__

#include "common.h"

typedef struct heap_header {
  enum {
    HEAP_BLOCK_FREE = 0xabababab,
    HEAP_BLOCK_USED = 0xcdcdcdcd
  } type;

  size_t size;
  struct heap_header *prev;
  struct heap_header *next;
} heap_header;

#define HEAP_SIZE 1024 * 1024 * 32

bool
heap_init();

void *
malloc(size_t size);

void
free(void *ptr);

#endif // __HEAP_H__  
