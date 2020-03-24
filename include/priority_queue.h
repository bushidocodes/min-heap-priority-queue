#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#define MAX 4096

/**
 * Compare the priority of two heap elements
 * @param a first element
 * @param b second element
 * @returns -1 if a < b, 0 if a == b, 1 if a > b
 **/
typedef int (*priority_queue_comparator_t)(void *a, void *b);

struct priority_queue {
  void *items[MAX];
  int first_free;
  priority_queue_comparator_t comparator;
};

void priority_queue_initialize(struct priority_queue *self,
                               priority_queue_comparator_t comparator);
int priority_queue_enqueue(struct priority_queue *self, void *value);
void *priority_queue_dequeue(struct priority_queue *self);
int priority_queue_length(struct priority_queue *self);

#endif /* PRIORITY_QUEUE_H */