#ifndef MEM_H
#define MEM_H

#include "atags.h"

#define PAGE_SIZE 4096

typedef struct {
  int allocated:1; // :1 indique qu'on utilise qu'un seul bit en fait, 0 ou 1 pour dire si alloué ou pas.
  int kernel_page:1; //pareil, pour savoir si fait partie du kernel 
  int reserved:30;
} page_flags_t;

typedef struct page {
  int vaddr_mapped; //addresse virtuelle qui correspond à cette page
  page_flags_t flags;
  struct page * next_page;
  struct page * prev_page;
} page_t;

void mem_init(atag_t *atags);

void *alloc_page();
void free_page(void *ptr);

void* malloc(uint32_t size);

#endif
