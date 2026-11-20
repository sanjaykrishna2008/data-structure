#include <stdio.h>

void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void sift_down(int a[], int n, int i)
{
    for (;;) {
        int l = 2*i + 1, r = l + 1, smallest = i;
        if (l < n && a[l] < a[smallest]) smallest = l;
        if (r < n && a[r] < a[smallest]) smallest = r;
        if (smallest == i) break;
        swap(&a[i], &a[smallest]);
        i = smallest;
    }
}

int main(void)
{
    int a[] = {9, 4, 7, 1, 8, 2, 6};
    int n = 7;

    for (int i = n/2 - 1; i >= 0; i--)
        sift_down(a, n, i);

    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}
