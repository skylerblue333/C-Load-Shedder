#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE 100

typedef struct {
    int request_id;
    int priority;
} Request;

Request queue[MAX_QUEUE];
int queue_size = 0;
int total_processed = 0;
int total_shed = 0;

int is_overloaded() {
    return queue_size > (MAX_QUEUE * 70 / 100);
}

void enqueue(int id, int priority) {
    if (queue_size < MAX_QUEUE) {
        queue[queue_size].request_id = id;
        queue[queue_size].priority = priority;
        queue_size++;
    }
}

void process_with_shedding() {
    for (int i = 0; i < queue_size; i++) {
        if (is_overloaded() && queue[i].priority < 5) {
            printf("  [SHED] Request #%d (priority %d) dropped.\n",
                   queue[i].request_id, queue[i].priority);
            total_shed++;
        } else {
            printf("  [OK]   Request #%d (priority %d) processed.\n",
                   queue[i].request_id, queue[i].priority);
            total_processed++;
        }
    }
    queue_size = 0;
}

int main() {
    srand(42);
    printf("=== Adaptive Load Shedder ===\n\n");

    for (int i = 1; i <= 80; i++) {
        enqueue(i, rand() % 10 + 1);
    }

    printf("Queue size: %d (threshold: %d)\n\n", queue_size, MAX_QUEUE * 70 / 100);
    process_with_shedding();

    printf("\n--- Summary ---\n");
    printf("Processed: %d\n", total_processed);
    printf("Shed:       %d\n", total_shed);
    return 0;
}
