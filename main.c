#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct {
    uint32_t capacity;
    uint32_t head;
    uint32_t tail;
    void *data[];
} queue_t;

queue_t *queue_create(uint32_t capacity) {
    queue_t *q = malloc(sizeof *q + capacity * sizeof *q->data);
    if (!q)
        return NULL;
    q->capacity = capacity;
    q->head = 0;
    q->tail = 0;
    return q;
}

void queue_destroy(queue_t *q) {
    free(q);
}

bool queue_empty(queue_t *q) {
    return q->head == q->tail;
}

bool queue_full(queue_t *q) {
    assert(q);
    uint32_t next_tail = (q->tail + 1) % q->capacity;
    return next_tail == q->head;
}

bool queue_enqueue(queue_t *q, void *item) {
    assert(q);
    if (queue_full(q))
        return false;
    q->data[q->tail] = item;
    q->tail = (q->tail + 1) % q->capacity;
    return true;
}

void *queue_dequeue(queue_t *q) {
    assert(q);
    if (queue_empty(q))
        return NULL;
    void *item = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    return item;
}

void visualize_queue(queue_t *q) {
    assert(q);
    InitWindow(800, 450, "Codotaku");
    SetTargetFPS(60);
    int next_val = 10;

    while (!WindowShouldClose()) {
        if (IsKeyReleased(KEY_Q))
            if (queue_enqueue(q, (void *) (uintptr_t) next_val))
                next_val += 10;
        if (IsKeyReleased(KEY_D)) queue_dequeue(q);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int i = 0; i < q->capacity; i++) {
            int x = 100 + (i * 70);
            int y = 200;
            DrawRectangleLines(x, y, 60, 60, BLACK);
            if (i == q->head) DrawText("HEAD", x, y - 30, 20, RED);
            if (i == q->tail) DrawText("TAIL", x, y + 70, 20, BLUE);
            if (q->data[i]) {
                DrawRectangle(x + 5, y + 5, 50, 50, GRAY);
                int val = (int) (uintptr_t) q->data[i];
                DrawText(TextFormat("%d", val), x + 15, y + 20, 20, BLACK);
            }

            if (queue_full(q)) {
                DrawText("QUEUE FULL", 100, 150, 20, RED);
            } else if (queue_empty(q)) {
                DrawText("QUEUE EMPTY", 100, 150, 20, DARKGRAY);
            } else {
                DrawText("QUEUE ACTIVE", 100, 150, 20, GREEN);
            }
        }

        EndDrawing();
    }
}

int main(void) {
    queue_t *q = queue_create(8);
    visualize_queue(q);
    queue_destroy(q);
    return 0;
}
