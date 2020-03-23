#include <assert.h>
#include <string.h>

#include "priority_queue.h"

/****************************
 * Private Helper Functions *
 ****************************/

/**
 * Adds a value to the end of the binary heap
 * @param self the priority queue
 * @param value the value we are adding
 * @return 0 on success. -1 when priority queue is full
 **/
static inline int priority_queue_append(struct priority_queue *self,
                                        int value) {
  assert(self != NULL);

  if (self->first_free >= MAX)
    return -1;

  memcpy(&self->items[self->first_free], &value, sizeof(int));
  self->first_free++;
  return 0;
}

/**
 * Shifts an appended value upwards to restore heap structure property
 * @param self the priority queue
 */
static inline void priority_queue_percolate_up(struct priority_queue *self) {
  assert(self != NULL);

  for (int i = self->first_free - 1; self->items[i] < self->items[i / 2];
       i /= 2) {
    int temp = self->items[i / 2];
    self->items[i / 2] = self->items[i];
    self->items[i] = temp;
  }
}

/**
 * Returns the index of a node's smallest child
 * @param self the priority queue
 * @param parent_index
 */
static inline int
priority_queue_find_smallest_child(struct priority_queue *self,
                                   int parent_index) {
  assert(self != NULL && parent_index >= 1 && parent_index < self->first_free);

  // If we don't have a right child or the left child is smaller, return it
  if (2 * parent_index + 1 == self->first_free) {
    return 2 * parent_index;
  } else if (self->items[2 * parent_index] <
             self->items[2 * parent_index + 1]) {
    return 2 * parent_index;
  } else {
    // Otherwise, return the right child
    return 2 * parent_index + 1;
  }
}

/**
 * Shifts the top of the heap downwares. Used after placing the last value at
 * the top
 * @param self the priority queue
 */
static inline void priority_queue_percolate_down(struct priority_queue *self) {
  assert(self != NULL);

  int i = 1;
  while (2 * i < self->first_free) {
    int smallest_child = priority_queue_find_smallest_child(self, i);
    // Once the parent is equal to or less than its smallest child, break;
    if (self->items[i] <= self->items[smallest_child])
      break;
    // Otherwise, swap and continue down the tree
    int temp = self->items[smallest_child];
    self->items[smallest_child] = self->items[i];
    self->items[i] = temp;

    i = smallest_child;
  }
}

/*********************
 * Public API        *
 *********************/

/**
 * Initialized the Priority Queue Data structure
 * @param self the priority_queue to initialize
 **/
void priority_queue_initialize(struct priority_queue *self) {
  assert(self != NULL);

  memset(self->items, 0, sizeof(int) * MAX);
  self->first_free = 1;
}

/**
 * @param self the priority_queue
 * @returns the number of elements in the priority queue
 **/
int priority_queue_length(struct priority_queue *self) {
  assert(self != NULL);

  return self->first_free - 1;
}

/**
 * @param self - the priority queue we want to add to
 * @param value - the value we want to add
 * @returns 0 on success. -1 when priority queue is full
 **/
int priority_queue_enqueue(struct priority_queue *self, int value) {
  assert(self != NULL);

  if (priority_queue_append(self, value) == -1)
    return -1;
  priority_queue_percolate_up(self);
  return 0;
}

/**
 * @param self - the priority queue we want to add to
 * @returns The head of the priority queue or -1 when empty
 **/
int priority_queue_dequeue(struct priority_queue *self) {
  assert(self != NULL);
  // If first_free is 1, we're empty
  if (self->first_free == 1)
    return -1;

  int min = self->items[1];
  self->items[1] = self->items[self->first_free - 1];
  self->items[self->first_free - 1] = 0;
  self->first_free--;
  if (self->first_free > 1)
    priority_queue_percolate_down(self);

  return min;
}
