#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"

struct sandbox_request {
  int absolute_deadline;
};

struct sandbox_request *sandbox_request_allocate(int absolute_deadline) {
  struct sandbox_request *sandbox_request =
      malloc(sizeof(struct sandbox_request));
  sandbox_request->absolute_deadline = absolute_deadline;
  return sandbox_request;
}

int sandbox_request_compare(void *a, void *b) {
  struct sandbox_request *first = (struct sandbox_request *)a;
  struct sandbox_request *second = (struct sandbox_request *)b;
  if (first->absolute_deadline < second->absolute_deadline) {
    return -1;
  } else if (first->absolute_deadline == second->absolute_deadline) {
    return 0;
  } else {
    return 1;
  }
}

void priority_queue_initialize_should_set_first_free_to_1() {
  struct priority_queue pq;
  priority_queue_initialize(&pq, sandbox_request_compare);
  assert(pq.first_free == 1);
}

void priority_queue_length_should_be_one_less_than_first_free() {
  struct priority_queue pq;
  priority_queue_initialize(&pq, sandbox_request_compare);
  assert(pq.first_free == 1);
  assert(priority_queue_length(&pq) == 0);
}

void priority_queue_enqueue_should_increment_first_free_and_length() {
  struct priority_queue pq;
  priority_queue_initialize(&pq, sandbox_request_compare);
  struct sandbox_request *sandbox_one = sandbox_request_allocate(10);
  priority_queue_enqueue(&pq, sandbox_one);
  assert(pq.first_free == 2);
  assert(priority_queue_length(&pq) == 1);
  free(sandbox_one);
}

void priority_queue_enqueue_first_call_should_set_index_1() {
  struct priority_queue pq;
  priority_queue_initialize(&pq, sandbox_request_compare);
  struct sandbox_request *sandbox_one = sandbox_request_allocate(10);
  priority_queue_enqueue(&pq, sandbox_one);
  assert(pq.items[1] == sandbox_one);
  free(sandbox_one);
}

void priority_queue_enqueue_returns_neg1_on_full() {
  struct priority_queue pq;
  priority_queue_initialize(&pq, sandbox_request_compare);
  struct sandbox_request *sandbox_one = sandbox_request_allocate(10);

  // Fill up the priority queue up to the max
  // This is one less than MAX because a min heap does not use the 0th element
  for (int i = 0; i < MAX - 1; i++)
    assert(priority_queue_enqueue(&pq, sandbox_one) == 0);

  // And then add one more
  assert(priority_queue_enqueue(&pq, sandbox_one) == -1);

  free(sandbox_one);
}

void priority_queue_dequeue_on_empty_returns_null() {
  struct priority_queue pq;
  priority_queue_initialize(&pq, sandbox_request_compare);
  assert(priority_queue_dequeue(&pq) == NULL);
}

void priority_queue_dequeue_of_one() {
  struct priority_queue pq;
  priority_queue_initialize(&pq, sandbox_request_compare);
  struct sandbox_request *sandbox_one = sandbox_request_allocate(10);
  priority_queue_enqueue(&pq, sandbox_one);
  assert(priority_queue_dequeue(&pq) == sandbox_one);
  free(sandbox_one);
}

void priority_queue_should_return_in_priority_order() {
  struct priority_queue pq;
  priority_queue_initialize(&pq, sandbox_request_compare);
  struct sandbox_request *sandbox_7 = sandbox_request_allocate(7);
  priority_queue_enqueue(&pq, sandbox_7);
  assert(pq.items[1] == sandbox_7);
  struct sandbox_request *sandbox_9 = sandbox_request_allocate(9);
  priority_queue_enqueue(&pq, sandbox_9);
  assert(pq.items[1] == sandbox_7);
  assert(pq.items[2] == sandbox_9);
  struct sandbox_request *sandbox_5 = sandbox_request_allocate(5);
  priority_queue_enqueue(&pq, sandbox_5);
  assert(pq.items[1] == sandbox_5);
  assert(pq.items[2] == sandbox_9);
  assert(pq.items[3] == sandbox_7);
  struct sandbox_request *sandbox_11 = sandbox_request_allocate(11);
  priority_queue_enqueue(&pq, sandbox_11);
  assert(pq.items[1] == sandbox_5);
  assert(pq.items[2] == sandbox_9);
  assert(pq.items[3] == sandbox_7);
  assert(pq.items[4] == sandbox_11);
  struct sandbox_request *sandbox_2 = sandbox_request_allocate(2);
  priority_queue_enqueue(&pq, sandbox_2);
  assert(pq.items[1] == sandbox_2);
  assert(pq.items[2] == sandbox_5);
  assert(pq.items[3] == sandbox_7);
  assert(pq.items[4] == sandbox_11);
  assert(pq.items[5] == sandbox_9);
  assert(priority_queue_dequeue(&pq) == sandbox_2);
  free(sandbox_2);
  assert(pq.items[1] == sandbox_5);
  assert(pq.items[2] == sandbox_9);
  assert(pq.items[3] == sandbox_7);
  assert(pq.items[4] == sandbox_11);
  assert(priority_queue_dequeue(&pq) == sandbox_5);
  free(sandbox_5);
  assert(pq.items[1] == sandbox_7);
  assert(pq.items[2] == sandbox_9);
  assert(pq.items[3] == sandbox_11);
  assert(priority_queue_dequeue(&pq) == sandbox_7);
  free(sandbox_7);
  assert(pq.items[1] == sandbox_9);
  assert(pq.items[2] == sandbox_11);
  assert(priority_queue_dequeue(&pq) == sandbox_9);
  free(sandbox_9);
  assert(pq.items[1] == sandbox_11);
  assert(priority_queue_dequeue(&pq) == sandbox_11);
  free(sandbox_11);
  assert(priority_queue_dequeue(&pq) == NULL);
}

int main(void) {
  priority_queue_initialize_should_set_first_free_to_1();
  priority_queue_length_should_be_one_less_than_first_free();
  priority_queue_enqueue_should_increment_first_free_and_length();
  priority_queue_enqueue_first_call_should_set_index_1();
  priority_queue_enqueue_returns_neg1_on_full();
  priority_queue_dequeue_on_empty_returns_null();
  priority_queue_dequeue_of_one();
  priority_queue_should_return_in_priority_order();

  printf("Passed All Tests!\n");
  return 0;
}