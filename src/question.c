#include <stdlib.h>
#include <string.h>


typedef struct hash_elem_t {
  struct hash_elem_t* next;
  void* data;
  char key[];
} hash_elem_t;

typedef struct {
  unsigned int capacity;
  unsigned int e_num;
  hash_elem_t** table;
} hashtable_t;



