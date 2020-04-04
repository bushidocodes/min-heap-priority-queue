#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "vendor/unity.h"
#include "priority_queue.h"

struct sandbox_request {
	unsigned long long int absolute_deadline;
};

struct sandbox_request *
sandbox_request_allocate(int absolute_deadline)
{
	struct sandbox_request *sandbox_request = malloc(sizeof(struct sandbox_request));
	sandbox_request->absolute_deadline      = absolute_deadline;
	return sandbox_request;
}

unsigned long long int
sandbox_request_get_priority(void *element_raw)
{
	struct sandbox_request *element = (struct sandbox_request *)element_raw;
	return element->absolute_deadline;
}

struct priority_queue pq;

void
setUp(void)
{
	priority_queue_initialize(&pq, sandbox_request_get_priority);
}

void
tearDown(void)
{
}

void
initialize_should_set_first_free_to_1()
{
	TEST_ASSERT_EQUAL_INT32(1, pq.first_free);
}

void
initialize_should_set_highest_priority_to_ULONG_MAX()
{
	TEST_ASSERT_EQUAL_UINT64(ULONG_MAX, pq.highest_priority);
}

void
length_should_be_one_less_than_first_free()
{
	TEST_ASSERT_EQUAL_INT32(pq.first_free - 1, priority_queue_length(&pq));
}

void
enqueue_should_increment_first_free_and_length()
{
	struct sandbox_request *sandbox_one = sandbox_request_allocate(10);
	priority_queue_enqueue(&pq, sandbox_one);
	TEST_ASSERT_EQUAL_INT32(2, pq.first_free);
	TEST_ASSERT_EQUAL_INT32(1, priority_queue_length(&pq));
	free(sandbox_one);
}

void
enqueue_first_call_should_set_highest_priority()
{
	struct sandbox_request *sandbox_one = sandbox_request_allocate(10);
	priority_queue_enqueue(&pq, sandbox_one);
	TEST_ASSERT_EQUAL_INT32(10, pq.highest_priority);
	free(sandbox_one);
}

void
enqueue_first_call_should_set_index_1()
{
	struct sandbox_request *sandbox_one = sandbox_request_allocate(10);
	priority_queue_enqueue(&pq, sandbox_one);
	TEST_ASSERT_EQUAL_PTR(sandbox_one, pq.items[1]);
	free(sandbox_one);
}

void
enqueue_returns_neg1_on_full()
{
	struct sandbox_request *sandbox_one = sandbox_request_allocate(10);

	// Fill up the priority queue up to the max
	// This is one less than MAX because a min heap does not use the 0th element
	for (int i = 0; i < MAX - 1; i++) TEST_ASSERT_EQUAL_INT32(0, priority_queue_enqueue(&pq, sandbox_one));

	// And then add one more
	TEST_ASSERT_EQUAL_INT32(-1, priority_queue_enqueue(&pq, sandbox_one));

	free(sandbox_one);
}

void
dequeue_on_empty_returns_null()
{
	TEST_ASSERT_EQUAL_PTR(NULL, priority_queue_dequeue(&pq));
}

void
dequeue_last_element_should_set_ULONG_MAX()
{
	TEST_ASSERT_EQUAL_UINT64(ULONG_MAX, pq.highest_priority);
	struct sandbox_request *sandbox_one = sandbox_request_allocate(10);
	priority_queue_enqueue(&pq, sandbox_one);
	TEST_ASSERT_LESS_THAN_UINT64(ULONG_MAX, pq.highest_priority);
	priority_queue_dequeue(&pq);
	free(sandbox_one);
	TEST_ASSERT_EQUAL_UINT64(ULONG_MAX, pq.highest_priority);
}

void
dequeue_of_one()
{
	struct sandbox_request *sandbox_one = sandbox_request_allocate(10);
	priority_queue_enqueue(&pq, sandbox_one);
	TEST_ASSERT_EQUAL_PTR(sandbox_one, priority_queue_dequeue(&pq));
	free(sandbox_one);
}

void
dequeue_should_return_in_priority_order()
{
	struct sandbox_request *sandbox_7 = sandbox_request_allocate(7);
	priority_queue_enqueue(&pq, sandbox_7);
	TEST_ASSERT_EQUAL_PTR(sandbox_7, pq.items[1]);
	TEST_ASSERT_EQUAL_UINT64(7, pq.highest_priority);

	struct sandbox_request *sandbox_9 = sandbox_request_allocate(9);
	priority_queue_enqueue(&pq, sandbox_9);
	TEST_ASSERT_EQUAL_PTR(sandbox_7, pq.items[1]);
	TEST_ASSERT_EQUAL_PTR(sandbox_9, pq.items[2]);
	TEST_ASSERT_EQUAL_UINT64(7, pq.highest_priority);

	struct sandbox_request *sandbox_5 = sandbox_request_allocate(5);
	priority_queue_enqueue(&pq, sandbox_5);
	TEST_ASSERT_EQUAL_PTR(sandbox_5, pq.items[1]);
	TEST_ASSERT_EQUAL_PTR(sandbox_9, pq.items[2]);
	TEST_ASSERT_EQUAL_PTR(sandbox_7, pq.items[3]);
	TEST_ASSERT_EQUAL_UINT64(5, pq.highest_priority);

	struct sandbox_request *sandbox_11 = sandbox_request_allocate(11);
	priority_queue_enqueue(&pq, sandbox_11);
	TEST_ASSERT_EQUAL_PTR(sandbox_5, pq.items[1]);
	TEST_ASSERT_EQUAL_PTR(sandbox_9, pq.items[2]);
	TEST_ASSERT_EQUAL_PTR(sandbox_7, pq.items[3]);
	TEST_ASSERT_EQUAL_PTR(sandbox_11, pq.items[4]);
	TEST_ASSERT_EQUAL_UINT64(5, pq.highest_priority);

	struct sandbox_request *sandbox_2 = sandbox_request_allocate(2);
	priority_queue_enqueue(&pq, sandbox_2);

	struct sandbox_request *state1[] = { NULL, sandbox_2, sandbox_5, sandbox_7, sandbox_11, sandbox_9 };
	TEST_ASSERT_EQUAL_PTR_ARRAY(state1, pq.items, 6);
	TEST_ASSERT_EQUAL_UINT64(2, pq.highest_priority);

	TEST_ASSERT_EQUAL_PTR(sandbox_2, priority_queue_dequeue(&pq));
	free(sandbox_2);
	struct sandbox_request *state2[] = { NULL, sandbox_5, sandbox_9, sandbox_7, sandbox_11 };
	TEST_ASSERT_EQUAL_PTR_ARRAY(state2, pq.items, 5);
	TEST_ASSERT_EQUAL_UINT64(5, pq.highest_priority);

	TEST_ASSERT_EQUAL_PTR(sandbox_5, priority_queue_dequeue(&pq));
	free(sandbox_5);
	struct sandbox_request *state3[] = { NULL, sandbox_7, sandbox_9, sandbox_11 };
	TEST_ASSERT_EQUAL_PTR_ARRAY(state3, pq.items, 4);
	TEST_ASSERT_EQUAL_UINT64(7, pq.highest_priority);

	TEST_ASSERT_EQUAL_PTR(sandbox_7, priority_queue_dequeue(&pq));
	free(sandbox_7);
	TEST_ASSERT_EQUAL_PTR(sandbox_9, pq.items[1]);
	TEST_ASSERT_EQUAL_PTR(sandbox_11, pq.items[2]);
	TEST_ASSERT_EQUAL_UINT64(9, pq.highest_priority);

	TEST_ASSERT_EQUAL_PTR(sandbox_9, priority_queue_dequeue(&pq));
	free(sandbox_9);
	TEST_ASSERT_EQUAL_PTR(sandbox_11, pq.items[1]);
	TEST_ASSERT_EQUAL_UINT64(11, pq.highest_priority);

	TEST_ASSERT_EQUAL_PTR(sandbox_11, priority_queue_dequeue(&pq));
	free(sandbox_11);
	TEST_ASSERT_EQUAL_PTR(NULL, pq.items[1]);
	TEST_ASSERT_EQUAL_UINT64(ULONG_MAX, pq.highest_priority);

	TEST_ASSERT_EQUAL_PTR(NULL, priority_queue_dequeue(&pq));
}

int
main(void)
{
	UnityBegin("priority_queue_test.c");
	RUN_TEST(initialize_should_set_first_free_to_1);
	RUN_TEST(initialize_should_set_highest_priority_to_ULONG_MAX);
	RUN_TEST(length_should_be_one_less_than_first_free);
	RUN_TEST(enqueue_first_call_should_set_highest_priority);
	RUN_TEST(enqueue_should_increment_first_free_and_length);
	RUN_TEST(enqueue_first_call_should_set_index_1);
	RUN_TEST(enqueue_returns_neg1_on_full);
	RUN_TEST(dequeue_on_empty_returns_null);
	RUN_TEST(dequeue_last_element_should_set_ULONG_MAX);
	RUN_TEST(dequeue_of_one);
	RUN_TEST(dequeue_should_return_in_priority_order);

	return UnityEnd();
}