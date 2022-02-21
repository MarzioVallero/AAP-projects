#ifndef _QUEUE_PRIVATE
#define _QUEUE_PRIVATE

#include "util.h"
#include "queuePublic.h"

/* structure declarations */
typedef struct node {
  void *data; //accepts any type of pointer! Amazing!
  struct node *next; //Pointer to the next element in the queue
} node_t;

struct queue { //Wrapper structure
  node_t *head; //Pointer to head element
  node_t *tail; //Pointer to tail element
  int num; //Number of elements currently in the queue, initialized to zero
};

#endif
