#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#define BUFFER_SIZE 10

typedef struct {
    int data[BUFFER_SIZE];
    _Atomic(size_t) read_pos;
    _Atomic(size_t) write_pos;
} ring_buffer_t;

bool ring_buffer_init(ring_buffer_t *buffer) {
    buffer->read_pos = 0;
    buffer->write_pos = 0;
    return true;
}

bool ring_buffer_write(ring_buffer_t *buffer, int value) {
    size_t curr_write_pos = atomic_load_explicit(&buffer->write_pos, memory_order_relaxed);
    size_t next_write_pos = (curr_write_pos + 1) % BUFFER_SIZE;
    if (next_write_pos == atomic_load_explicit(&buffer->read_pos, memory_order_acquire)) {
        return false; // buffer full
    }
    buffer->data[curr_write_pos] = value;
    atomic_store_explicit(&buffer->write_pos, next_write_pos, memory_order_release);
    return true;
}

bool ring_buffer_read(ring_buffer_t *buffer, int *value) {
    size_t curr_read_pos = atomic_load_explicit(&buffer->read_pos, memory_order_relaxed);
    if (curr_read_pos == atomic_load_explicit(&buffer->write_pos, memory_order_acquire)) {
        return false; // buffer empty
    }
    *value = buffer->data[curr_read_pos];
    size_t next_read_pos = (curr_read_pos + 1) % BUFFER_SIZE;
    atomic_store_explicit(&buffer->read_pos, next_read_pos, memory_order_release);
    return true;
}

int main() {
    ring_buffer_t buffer = {{1,23,4,6,23,45,8}};
    if (!ring_buffer_init(&buffer)) {
        fprintf(stderr, "Failed to initialize ring buffer.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (!ring_buffer_write(&buffer, i)) {
            fprintf(stderr, "Failed to write data to ring buffer.\n");
            exit(EXIT_FAILURE);
        }
    }
    int value;
    while (ring_buffer_read(&buffer, &value)) {
        printf("%d ", value);
    }
    printf("\n");
    return 0;
}

