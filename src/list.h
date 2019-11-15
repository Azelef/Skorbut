#ifndef LIST
#define LIST

#define DEFINE_LIST(nodetype)			\
  typedef struct nodetype ## list nodetype##_list;	\
  struct nodetype##list {				\
    nodetype * head;					\
    nodetype##_list * tail;				\
    int size;						\
  };

#define INITIALIZE_LIST(list)			\
  list.head = (void *) 0; list.tail = (void *) 0; list.size = 0;

# define IMPLEMENT_LIST(nodetype)					\
  void app_##nodetype(nodetype##_list *list, nodetype *node) {		\
    if (list->size == 0) {						\
      list->head = node; list->tail = (void *) 0; list->size = 1;	\
    }									\
    else {								\
      list->head = node; list->tail = list; list->size = list->size + 1; \
    }									\
  }									\
  									\
  nodetype *pop_##nodetype(nodetype##_list *list) {			\
    nodetype *node = list->head;					\
    list = list->tail;							\
    return node;							\
  }									\
  									\
  int size_##nodetype(nodetype##_list *list) {				\
    return list->size;							\
  }								


#endif
