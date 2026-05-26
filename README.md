# Min Heap Priority Queue

A generic min-heap priority queue in C11. Elements are `void *` pointers; ordering is determined by a caller-supplied key function. The heap is backed by a static array of configurable capacity.

## Features

- Generic: stores any pointer type via `void *` with a `uint64_t` key callback
- Static allocation: no heap allocation, capacity fixed at compile time
- Configurable capacity: override `PRIORITY_QUEUE_CAPACITY` at compile time (default: 4096)
- Compile-time safety: `static_assert` bounds on capacity to prevent index overflow
- Cached minimum: `min_key` field tracks the root key without an extra lookup
- C11, `-Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror` clean

## API

```c
// Initialize a queue with a key-extraction callback
void priority_queue_initialize(struct priority_queue *self, priority_queue_get_key_t get_key);

// Reset to empty, preserving the get_key callback
void priority_queue_clear(struct priority_queue *self);

// Add an element; returns 0 on success, -1 if full
int priority_queue_enqueue(struct priority_queue *self, void *value);  // return value must be checked

// Remove and return the minimum-key element, or NULL if empty
void *priority_queue_dequeue(struct priority_queue *self);

// Return the minimum-key element without removing it, or NULL if empty
void *priority_queue_peek(const struct priority_queue *self);

size_t priority_queue_length(const struct priority_queue *self);
bool   priority_queue_is_empty(const struct priority_queue *self);
bool   priority_queue_is_full(const struct priority_queue *self);
```

The key callback type:

```c
typedef uint64_t (*priority_queue_get_key_t)(void *element);
```

## Usage

```c
#include "priority_queue.h"

uint64_t get_deadline(void *element) {
    return ((struct task *)element)->deadline;
}

struct priority_queue pq;
priority_queue_initialize(&pq, get_deadline);

if (priority_queue_enqueue(&pq, task) != 0) {
    // queue full
}

struct task *next = priority_queue_dequeue(&pq);
```

## Configurable Capacity

The default capacity is 4096. Override it at compile time:

```
make CFLAGS="-DPRIORITY_QUEUE_CAPACITY=256"
```

Valid range: `2 ≤ PRIORITY_QUEUE_CAPACITY ≤ SIZE_MAX/2`. Both bounds are enforced at compile time.

## Building and Testing

```
make test       # build and run tests
make memcheck   # build and run with AddressSanitizer + UndefinedBehaviorSanitizer
make format     # run clang-format
```

Requires `clang`.
