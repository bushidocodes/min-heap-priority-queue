#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#define MAX 4096

struct priority_queue {
  int items[MAX];
  int first_free;
};

void priority_queue_initialize(struct priority_queue *self);
int priority_queue_enqueue(struct priority_queue *self, int value);
int priority_queue_dequeue(struct priority_queue *self);
int priority_queue_length(struct priority_queue *self);

#endif /* PRIORITY_QUEUE_H */