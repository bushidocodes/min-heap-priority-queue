#include <assert.h>
#include <stdio.h>

#include "priority_queue.h"

void priority_queue_initialize_should_set_first_free_to_1() {
  struct priority_queue pq;
  priority_queue_initialize(&pq);
  assert(pq.first_free == 1);
}

void priority_queue_length_should_be_one_less_than_first_free() {
  struct priority_queue pq;
  priority_queue_initialize(&pq);
  assert(pq.first_free == 1);
  assert(priority_queue_length(&pq) == 0);
}

void priority_queue_enqueue_should_increment_first_free_and_length() {
  struct priority_queue pq;
  priority_queue_initialize(&pq);
  priority_queue_enqueue(&pq, 10);
  assert(pq.first_free == 2);
  assert(priority_queue_length(&pq) == 1);
}

void priority_queue_enqueue_first_call_should_set_index_1() {
  struct priority_queue pq;
  priority_queue_initialize(&pq);
  priority_queue_enqueue(&pq, 10);
  assert(pq.items[1] == 10);
}

void priority_queue_enqueue_returns_neg1_on_full() {
  struct priority_queue pq;
  priority_queue_initialize(&pq);

  // Fill up the priority queue up to the max
  // This is one less than MAX because a min heap does not use the 0th element
  for (int i = 0; i < MAX - 1; i++)
    assert(priority_queue_enqueue(&pq, 10) == 0);

  // And then add one more
  assert(priority_queue_enqueue(&pq, 10) == -1);
}

void priority_queue_dequeue_on_empty_returns_neg1() {
  struct priority_queue pq;
  priority_queue_initialize(&pq);
  assert(priority_queue_dequeue(&pq) == -1);
}

void priority_queue_dequeue_of_one() {
  struct priority_queue pq;
  priority_queue_initialize(&pq);
  priority_queue_enqueue(&pq, 10);
  assert(priority_queue_dequeue(&pq) == 10);
}

void priority_queue_should_return_in_priority_order() {
  struct priority_queue pq;
  priority_queue_initialize(&pq);
  priority_queue_enqueue(&pq, 7);
  assert(pq.items[1] == 7);
  priority_queue_enqueue(&pq, 9);
  assert(pq.items[1] == 7);
  assert(pq.items[2] == 9);
  priority_queue_enqueue(&pq, 5);
  assert(pq.items[1] == 5);
  assert(pq.items[2] == 9);
  assert(pq.items[3] == 7);
  priority_queue_enqueue(&pq, 11);
  assert(pq.items[1] == 5);
  assert(pq.items[2] == 9);
  assert(pq.items[3] == 7);
  assert(pq.items[4] == 11);
  priority_queue_enqueue(&pq, 2);
  assert(pq.items[1] == 2);
  assert(pq.items[2] == 5);
  assert(pq.items[3] == 7);
  assert(pq.items[4] == 11);
  assert(pq.items[5] == 9);
  assert(priority_queue_dequeue(&pq) == 2);
  assert(pq.items[1] == 5);
  assert(pq.items[2] == 9);
  assert(pq.items[3] == 7);
  assert(pq.items[4] == 11);
  assert(priority_queue_dequeue(&pq) == 5);
  assert(pq.items[1] == 7);
  assert(pq.items[2] == 9);
  assert(pq.items[3] == 11);
  assert(priority_queue_dequeue(&pq) == 7);
  assert(pq.items[1] == 9);
  assert(pq.items[2] == 11);
  assert(priority_queue_dequeue(&pq) == 9);
  assert(pq.items[1] == 11);
  assert(priority_queue_dequeue(&pq) == 11);
  assert(priority_queue_dequeue(&pq) == -1);
}

int main(void) {
  priority_queue_initialize_should_set_first_free_to_1();
  priority_queue_length_should_be_one_less_than_first_free();
  priority_queue_enqueue_should_increment_first_free_and_length();
  priority_queue_enqueue_first_call_should_set_index_1();
  priority_queue_enqueue_returns_neg1_on_full();
  priority_queue_dequeue_on_empty_returns_neg1();
  priority_queue_dequeue_of_one();
  priority_queue_should_return_in_priority_order();

  printf("Passed All Tests!\n");
  return 0;
}