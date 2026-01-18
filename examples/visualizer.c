#define CODOTAKU_DS_IMPL
#include <codotaku_ds/queue.h>
#include <raylib.h>

void visualize_queue(queue_t *q) {
    InitWindow(1000, 600, "Codotaku - Ultimate Circular Queue Visualizer");
    SetTargetFPS(60);

    int next_val = 10;
    void *linear_preview[16];

    while (!WindowShouldClose()) {
        if (IsKeyReleased(KEY_Q))
            if (queue_enqueue(q, (void *) (uintptr_t) next_val))
                next_val += 10;

        if (IsKeyReleased(KEY_D))
            queue_dequeue(q);

        uint32_t count = queue_to_array(q, linear_preview, 16);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("CONTROLS: [Q] Enqueue  [D] Dequeue", 20, 20, 20, DARKGRAY);

        DrawText(TextFormat("Count: %u", queue_count(q)), 20, 60, 20, BLACK);
        DrawText(TextFormat("Capacity: %u", queue_capacity(q)), 20, 90, 20, BLACK);
        DrawText(queue_linear(q) ? "State: LINEAR" : "State: WRAPPED", 20, 120, 20, BLUE);

        DrawText("PHYSICAL BUFFER (Raw Memory)", 300, 160, 18, GRAY);
        for (uint32_t i = 0; i < queue_capacity(q); i++) {
            int x = 300 + (i * 80);
            int y = 200;
            bool active = queue_active(q, i);
            Color boxColor = active ? SKYBLUE : LIGHTGRAY;

            DrawRectangle(x, y, 70, 70, boxColor);
            DrawRectangleLines(x, y, 70, 70, BLACK);

            if (i == q->head) DrawText("HEAD", x + 10, y - 25, 18, RED);
            if (i == q->tail) DrawText("TAIL", x + 10, y + 80, 18, DARKBLUE);

            if (active) {
                int val = (int) (uintptr_t) q->data[i];
                DrawText(TextFormat("%d", val), x + 20, y + 25, 20, BLACK);
            }

            DrawText(TextFormat("[%d]", i), x + 25, y + 110, 15, GRAY);
        }

        DrawLine(50, 400, 950, 400, LIGHTGRAY);
        DrawText("LINEARIZED OUTPUT (queue_to_array result)", 300, 420, 18, DARKGREEN);
        for (uint32_t i = 0; i < count; i++) {
            int x = 300 + (i * 80);
            int y = 460;

            DrawRectangle(x, y, 60, 60, LIME);
            DrawRectangleLines(x, y, 60, 60, DARKGREEN);

            int val = (int) (uintptr_t) linear_preview[i];
            DrawText(TextFormat("%d", val), x + 15, y + 20, 20, BLACK);
            DrawText(TextFormat("idx %d", i), x + 10, y + 65, 15, DARKGRAY);
        }

        if (queue_full(q)) DrawText("FULL!", 850, 60, 30, RED);
        if (queue_empty(q)) DrawText("EMPTY", 850, 60, 30, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
}

int main(void) {
    queue_t q;
    void *data[8];
    queue_init(&q, (void **) &data, sizeof data / sizeof *data);
    visualize_queue(&q);
    return 0;
}
