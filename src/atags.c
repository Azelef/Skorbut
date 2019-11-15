#include "atags.h"

int get_mem_size(atag_t* Tag) {
  while (Tag->tag != NONE){
    if (Tag->tag == MEM) {
      return Tag->mem.size;
    }
    Tag = (Tag) + (Tag->tag_size);
  }
  return 0;
}
