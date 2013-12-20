#include "heap.h"
#include "stdio.h"

static heap_header *header_list;

static int
brk(const void *end_data_segment)
{
  int status = 0;

  asm("movl $45, %%eax \n\t"
      "movl %1, %%ebx \n\t"
      "int $0x80 \n\t"
      "movl %%eax, %0 \n\t"
      :"=r"(status)
      :"m"(end_data_segment)
     );
  return status;
}

bool
heap_init()
{
  void *base = (void *)brk(0);
  void *end = base + HEAP_SIZE;
  
  end = (void *)brk(end);
  if (end == NULL) {
    return false;
  }
  heap_header *header = (heap_header *)base;

  header->size = HEAP_SIZE;
  header->type = HEAP_BLOCK_FREE;
  header->prev = header->next = NULL;

  header_list = header;

  return true;
}

void *
malloc(size_t size)
{
  if (size == 0) {
    return NULL;
  }
  heap_header *header = header_list;

  while(header != NULL) {
    if (header->type == HEAP_BLOCK_USED) {
      header = header->next;
      continue;
    }
    if (header->size > size) {
      heap_header *next_header = (heap_header *)(header + size);
      next_header->prev = header;
      next_header->next = header->next;
      next_header->type = HEAP_BLOCK_FREE;
      next_header->size = header->size - size;
      header->next = next_header;
      header->type = HEAP_BLOCK_USED;
      header->size = size;
      break;
    } else if (header->size == size) {
      header->type = HEAP_BLOCK_USED;
      break;
    }
    header = header->next;
  }
  return header;
}

void
free(void *ptr)
{
  heap_header *header = (heap_header *)ptr;

  if (header->type != HEAP_BLOCK_USED) {
    return;
  }
  header->type = HEAP_BLOCK_FREE;

  if (header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE) {
    header->prev->next = header->next;
    if (header->next != NULL) {
      header->next->prev = header->prev;
    }
    header->prev->size += header->size;
    header = header->prev;
  }
  if (header->next != NULL && header->next->type == HEAP_BLOCK_FREE) {
    header->size += header->next->size;
    header->next = header->next->next;
  }
}
