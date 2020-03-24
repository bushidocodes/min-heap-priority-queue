#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "priority_queue.h"

/****************************
 * Private Helper Functions *
 ****************************/

/**
 * Adds a value to the end of the binary heap
 * @param self the priority queue
 * @param new_item the value we are adding
 * @return 0 on success. -1 when priority queue is full
 **/
static inline int priority_queue_append(struct priority_queue *self,
                                        void *new_item) {
  assert(self != NULL);

  if (self->first_free >= MAX)
    return -1;

  self->items[self->first_free] = new_item;
  self->first_free++;
  return 0;
}

/**
 * Shifts an appended value upwards to restore heap structure property
 * @param self the priority queue
 */
static inline void priority_queue_percolate_up(struct priority_queue *self) {
  assert(self != NULL);
  assert(self->comparator != NULL);

  for (int i = self->first_free - 1;
       i / 2 != 0 && self->comparator(self->items[i], self->items[i / 2]) == -1;
       i /= 2) {
    void *temp = self->items[i / 2];
    self->items[i / 2] = self->items[i];
    self->items[i] = temp;
  }
}

/**
 * Returns the index of a node's smallest child
 * @param self the priority queue
 * @param parent_index
 * @returns the index of the smallest child
 */
static inline int
priority_queue_find_smallest_child(struct priority_queue *self,
                                   int parent_index) {
  assert(self != NULL);
  assert(parent_index >= 1 && parent_index < self->first_free);
  assert(self->comparator != NULL);

  int left_child_index = 2 * parent_index;
  int right_child_index = 2 * parent_index + 1;
  assert(self->items[left_child_index] != NULL);

  // If we don't have a right child or the left child is smaller, return it
  if (right_child_index == self->first_free) {
    return left_child_index;
  } else if (self->comparator(self->items[left_child_index],
                              self->items[right_child_index]) == -1) {
    return left_child_index;
  } else {
    // Otherwise, return the right child
    return right_child_index;
  }
}

/**
 * Shifts the top of the heap downwards. Used after placing the last value at
 * the top
 * @param self the priority queue
 */
static inline void priority_queue_percolate_down(struct priority_queue *self) {
  assert(self != NULL);

  int parent_index = 1;
  int left_child_index = 2 * parent_index;
  while (left_child_index >= 2 && left_child_index < self->first_free) {
    int smallest_child_index =
        priority_queue_find_smallest_child(self, parent_index);
    // Once the parent is equal to or less than its smallest child, break;
    if (self->comparator(self->items[parent_index],
                         self->items[smallest_child_index]) <= 0)
      break;
    // Otherwise, swap and continue down the tree
    void *temp = self->items[smallest_child_index];
    self->items[smallest_child_index] = self->items[parent_index];
    self->items[parent_index] = temp;

    parent_index = smallest_child_index;
    left_child_index = 2 * parent_index;
  }
}

/*********************
 * Public API        *
 *********************/

/**
 * Initialized the Priority Queue Data structure
 * @param self the priority_queue to initialize
 * @param comparator pointer to a function that can compare what we're storing
 **/
void priority_queue_initialize(struct priority_queue *self,
                               priority_queue_comparator_t comparator) {
  assert(self != NULL);
  assert(comparator != NULL);

  memset(self->items, 0, sizeof(void *) * MAX);
  self->first_free = 1;
  self->comparator = comparator;
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
int priority_queue_enqueue(struct priority_queue *self, void *value) {
  assert(self != NULL);

  if (priority_queue_append(self, value) == -1)
    return -1;
  priority_queue_percolate_up(self);
  return 0;
}

/**
 * @param self - the priority queue we want to add to
 * @returns The head of the priority queue or NULL when empty
 **/
void *priority_queue_dequeue(struct priority_queue *self) {
  assert(self != NULL);
  // If first_free is 1, we're empty
  if (self->first_free == 1)
    return NULL;

  void *min = self->items[1];
  self->items[1] = self->items[self->first_free - 1];
  self->items[self->first_free - 1] = NULL;
  self->first_free--;
  assert(self->first_free == 1 || self->items[self->first_free - 1] != NULL);
  // Because of 1-based indices, first_free is 2 when there is only one element
  if (self->first_free > 2)
    priority_queue_percolate_down(self);

  return min;
}
