#include "queuePrivate.h"
#include "util.h"

/*
 *  create a new empty queue
 */
queue_t *queue_init(int size)
{
  queue_t *qp;

  qp = (queue_t *)util_malloc(sizeof(queue_t)); //Dynamic allocation
  qp->head = qp->tail = NULL; //Both pointers are null, the queue is empty on initialization
  qp->num = 0; //There are no elements in the queue at startup
  return qp;
}

/*
 *  return the number of elements stored in the queue
 */
int queue_count(queue_t *qp)
{
  return (qp!=NULL) ? qp->num : 0;
}

/*
 *  store a new value in the queue (FIFO policy)
 */
int queue_put(queue_t *qp, void *data)
{
  node_t *node;

  if (qp == NULL) {
    return 0; //Failure!
  }

  node = (node_t *)malloc(sizeof(node_t)); //Dynamic allocation of a queue element
  node->data = data; //copy
  node->next = NULL; //copy

  if (qp->head == NULL) {
    qp->head = node; //Case the queue is empty
  } else {
    qp->tail->next = node; //In all other cases, add on the tail
  }
  qp->tail = node; //Update tail
  qp->num++; //Increase number of elements in the queue

  return 1; //Success!
}

/*
 *  extract a value from the queue (FIFO policy)
 */
int queue_get(queue_t *qp, void **data_ptr)
{
  node_t *node;

  if (qp==NULL || qp->head==NULL) {
    return 0; //Failure!
  }

  node = qp->head; //Assign head to a dummy pointer
  *data_ptr = node->data; //data_ptr now has obtainted the requested data from the queue
  qp->head = node->next; //head has to point to the next element at the start of the queue
  if (qp->head == NULL) {//If last element has been extracted, then
    qp->tail = NULL; //    there's no need for a tail pointer anymore
  }
  qp->num--; //Decrease the number of elements present in the queue each time we extract one

  free(node);
  return 1; //Success!
}

/*
 *  print all the queue elements (FIFO policy)
 */
void queue_print(FILE *fp, queue_t *qp, void (*print)(FILE *, void *))
{
  node_t *node;

  if (qp != NULL) {
    node = qp->head;
    while (node != NULL) {
      print(fp, node->data);
      fprintf (fp, "\n");
      node = node->next;
    }
  }
}

/*
 *  deallocate all the memory associated to the queue
 */
void queue_dispose(queue_t *qp, void (*quit)(void *))
{
  node_t *node;

  if (qp != NULL) {
    while (qp->head != NULL) {
      node = qp->head;
      qp->head = node->next;
      if (quit != NULL) {
        quit(node->data);
      }
      free(node);
    }
    free(qp);
  }
}
