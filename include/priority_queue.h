#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#ifndef PRIORITY_QUEUE_CAPACITY
#define PRIORITY_QUEUE_CAPACITY 4096
#endif

#if PRIORITY_QUEUE_CAPACITY < 2
#error "PRIORITY_QUEUE_CAPACITY must be at least 2 (index 0 is unused in the 1-indexed heap)"
#endif

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Child indices are computed as 2*i and 2*i+1 using size_t arithmetic.
 * SIZE_MAX is always odd (2^N - 1 for an N-bit type), so at i = SIZE_MAX/2:
 * 2*i = SIZE_MAX-1 and 2*i+1 = SIZE_MAX — both representable, no overflow.
 * Capacities above SIZE_MAX/2 would allow i to exceed this bound. */
static_assert(PRIORITY_QUEUE_CAPACITY <= SIZE_MAX / 2,
              "PRIORITY_QUEUE_CAPACITY must be at most SIZE_MAX/2 to avoid size_t overflow in child index calculations");
static_assert(SIZE_MAX % 2 == 1, "SIZE_MAX must be odd (size_t uses pure binary representation per C11 §6.2.6.2)");

#if defined(__GNUC__) || defined(__clang__)
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif

/**
 * How to get the priority out of the generic element
 * Extracts the ordering key from an element as a uint64_t (e.g. deadline in
 * cycles or UNIX time in ms). Used to maintain a cached copy of the minimum
 * key currently in the heap.
 * @param element
 * @returns priority (a u64)
 **/
typedef uint64_t (*priority_queue_get_key_t)(void *element);

struct priority_queue {
	uint64_t                      min_key; /* cached key of the heap root */
	void                         *items[PRIORITY_QUEUE_CAPACITY];
	size_t                        first_free;
	priority_queue_get_key_t get_key;
};

void   priority_queue_initialize(struct priority_queue *const self, priority_queue_get_key_t get_key);
void   priority_queue_clear(struct priority_queue *const self);
WARN_UNUSED_RESULT int priority_queue_enqueue(struct priority_queue *const self, void *value);
void  *priority_queue_dequeue(struct priority_queue *const self);
void  *priority_queue_peek(const struct priority_queue *const self);
size_t priority_queue_length(const struct priority_queue *const self);
bool   priority_queue_is_empty(const struct priority_queue *const self);
bool   priority_queue_is_full(const struct priority_queue *const self);

#endif /* PRIORITY_QUEUE_H */
