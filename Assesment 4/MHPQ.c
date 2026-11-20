#include <stdio.h>
#define MAX 32

typedef struct { int severity; int patient; } Case;
Case heap[MAX];
int size = 0;

void swap(Case *a, Case *b)
{
    Case t = *a; *a = *b; *b = t;
}

void push(int severity, int patient)
{
    if (size == MAX) return;
    heap[size] = (Case){severity, patient};
    int i = size++;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[parent].severity >= heap[i].severity) break;
        swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

Case pop(void)
{
    if (size == 0) return (Case){0, 0};
    Case top = heap[0];
    heap[0] = heap[--size];
    int i = 0;
    for (;;) {
        int left = 2 * i + 1;
        int right = left + 1;
        int largest = i;
        if (left < size && heap[left].severity > heap[largest].severity)
            largest = left;
        if (right < size && heap[right].severity > heap[largest].severity)
            largest = right;
        if (largest == i) break;
        swap(&heap[i], &heap[largest]);
        i = largest;
    }
    return top;
}

int main(void)
{
    int sev[] = {3, 9, 5, 7, 1, 8};
    int pid[] = {101, 102, 103, 104, 105, 106};
    for (int i = 0; i < 6; i++)
        push(sev[i], pid[i]);

    while (size > 0) {
        Case c = pop();
        printf("P%d(%d) ", c.patient, c.severity);
    }
    printf("\n");
    return 0;
}
