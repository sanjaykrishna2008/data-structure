#include <stdio.h>

typedef struct { int order_id; int amount; } Order;
long comparisons = 0;

void merge(Order a[], Order tmp[], int lo, int mid, int hi)
{
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        ++comparisons;
        if (a[i].amount <= a[j].amount)
            tmp[k++] = a[i++];
        else
            tmp[k++] = a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi)  tmp[k++] = a[j++];
    for (int x = lo; x <= hi; ++x) a[x] = tmp[x];
}

void merge_sort(Order a[], Order tmp[], int lo, int hi)
{
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort(a, tmp, lo, mid);
    merge_sort(a, tmp, mid + 1, hi);
    merge(a, tmp, lo, mid, hi);
}

int main(void)
{
    Order a[] = {
        {1,450}, {2,120}, {3,450}, {4,990},
        {5,120}, {6,300}, {7,780}, {8,300}
    };
    Order tmp[8];
    int n = 8;

    merge_sort(a, tmp, 0, n-1);

    for (int i = 0; i < n; i++)
        printf("#%d:%d ", a[i].order_id, a[i].amount);
    printf("\ncomparisons = %ld\n", comparisons);
    return 0;
}
