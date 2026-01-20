#pragma once
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void **data;
    uint32_t mask;
    uint32_t head;
    uint32_t tail;
} queue_t;

bool queue_init(queue_t *q, void **data, uint32_t capacity);

queue_t *queue_create(uint32_t capacity);

void queue_destroy(queue_t *q);

void queue_clear(queue_t *q);

bool queue_empty(const queue_t *q);

bool queue_full(const queue_t *q);

bool queue_linear(const queue_t *q);

uint32_t queue_capacity(const queue_t *q);

uint32_t queue_count(const queue_t *q);

bool queue_active(const queue_t *q, uint32_t i);

bool queue_enqueue(queue_t *q, void *item);

void *queue_dequeue(queue_t *q);

void *queue_peek(const queue_t *q);

uint32_t queue_to_array(const queue_t *restrict q, void **restrict dest,
                        uint32_t dest_size);

// #define CODOTAKU_DS_IMPL
#ifdef CODOTAKU_DS_IMPL

bool queue_init(queue_t *q, void **data, uint32_t capacity) {
    assert(q);
    assert(data);
    assert(capacity > 1);
    assert((capacity & (capacity - 1)) == 0 &&
           "capacity must be a power of two");

    q->mask = capacity - 1;
    q->head = 0;
    q->tail = 0;
    q->data = data;
    return true;
}

queue_t *queue_create(uint32_t capacity) {
    assert(capacity > 1);
    assert((capacity & (capacity - 1)) == 0 &&
           "capacity must be a power of two");

    size_t header_size = sizeof(queue_t);
    size_t data_size = capacity * sizeof(void *);

    uint8_t *block = malloc(header_size + data_size);
    if (!block)
        return NULL;

    queue_t *q = (queue_t *)block;
    void **data_ptr = (void **)(block + header_size);

    queue_init(q, data_ptr, capacity);
    return q;
}

void queue_destroy(queue_t *q) { free(q); }

void queue_clear(queue_t *q) {
    assert(q);
    q->head = q->tail;
}

bool queue_empty(const queue_t *q) {
    assert(q);
    return q->head == q->tail;
}

bool queue_full(const queue_t *q) {
    assert(q);
    uint32_t next_tail = (q->tail + 1) & q->mask;
    return next_tail == q->head;
}

bool queue_linear(const queue_t *q) {
    assert(q);
    return q->head <= q->tail;
}

uint32_t queue_capacity(const queue_t *q) {
    assert(q);
    return q->mask + 1;
}

uint32_t queue_count(const queue_t *q) {
    assert(q);
    return (q->tail - q->head) & q->mask;
}

bool queue_active(const queue_t *q, uint32_t i) {
    assert(q);
    uint32_t dist_from_head = (i - q->head) & q->mask;
    return dist_from_head < queue_count(q);
}

bool queue_enqueue(queue_t *q, void *item) {
    assert(q);
    if (queue_full(q))
        return false;
    q->data[q->tail] = item;
    q->tail = (q->tail + 1) & q->mask;
    return true;
}

void *queue_dequeue(queue_t *q) {
    assert(q);
    if (queue_empty(q))
        return NULL;
    void *item = q->data[q->head];
    q->head = (q->head + 1) & q->mask;
    return item;
}

void *queue_peek(const queue_t *q) {
    assert(q);
    if (queue_empty(q))
        return NULL;
    return q->data[q->head];
}

uint32_t queue_to_array(const queue_t *restrict q, void **restrict dest,
                        uint32_t dest_size) {
    assert(q);
    assert(dest);

    uint32_t count = queue_count(q);
    uint32_t to_copy = (count < dest_size) ? count : dest_size;

    if (to_copy == 0)
        return 0;

    if (queue_linear(q)) {
        memcpy(dest, &q->data[q->head], to_copy * sizeof(void *));
    } else {
        uint32_t head_side_count = queue_capacity(q) - q->head;
        uint32_t first_copy =
            (to_copy < head_side_count) ? to_copy : head_side_count;
        memcpy(dest, &q->data[q->head], first_copy * sizeof(void *));

        if (to_copy > first_copy) {
            uint32_t remaining = to_copy - first_copy;
            memcpy(&dest[first_copy], &q->data[0], remaining * sizeof(void *));
        }
    }

    return to_copy;
}

#endif
