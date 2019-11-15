#include <stddef.h>
#include <stdint.h>
#include "atags.h"
#include "mem.h"
#include "list.h"


DEFINE_LIST(page_t)
IMPLEMENT_LIST(page_t)
extern int __end;
static int num_pages;

static page_t *all_pages_array;
page_t_list free_pages;

void mem_init(atag_t *atags) {
  int mem_size, kernel_pages, i; //page_array_len;

  mem_size = get_mem_size(atags);
  num_pages = mem_size/PAGE_SIZE;

  //  page_array_len = sizeof(page_t)*num_pages;
  all_pages_array = (page_t*)&__end; //adresse de __end
  // memset(all_pages_array, 0, page_array_len); //initialise un bloc de taille page_array_len d'octets à la valeur 2e arg, à partir de l'adresse all_pages_array
  INITIALIZE_LIST(free_pages)

  kernel_pages = ((int)&__end)/PAGE_SIZE; //nombre de page pour le kernel, les autres seront libres
  for (i = 0; i < kernel_pages; i++) {
    all_pages_array[i].vaddr_mapped = i*PAGE_SIZE; //c'est un pointeur donc un tableau eh!
    all_pages_array[i].flags.allocated = 1;
    all_pages_array[i].flags.kernel_page = 1;
  }
  //le reste des pages :
  for (;i < num_pages; i++) {
    all_pages_array[i].flags.allocated = 0;
    app_page_t(&free_pages, &all_pages_array[i]);
  }
}

void *alloc_page() {
  page_t *page;
  void *page_mem;

  if (size_page_t(&free_pages)==0) {
    return 0;
  }

  page = pop_page_t(&free_pages);
  page->flags.kernel_page = 1; 
  page->flags.allocated = 1;

  page_mem = (void *)((page - all_pages_array)*PAGE_SIZE);
  //  memset(page_mem, 0, PAGE_SIZE);
  return page_mem;
}

void free_page(void *ptr) {
  page_t *page;

  page = all_pages_array + ((int)ptr/PAGE_SIZE);
  page->flags.allocated = 0;
  app_page_t(&free_pages,page);
}

uint32_t ptr=0x00420000;
void* malloc(uint32_t size)
{
    void* r=(void*)ptr;
    ptr+=size;
    return r;
}
