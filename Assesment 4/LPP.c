#include <stdio.h>
#define M 10

int table[M];

void insert(int key)
{
    int idx = key % M;
    int probes = 1;
    while (table[idx] != 0) {
        idx = (idx + 1) % M;
        probes++;
    }
    table[idx] = key;
    printf("%d@%d(%d) ", key, idx, probes);
}

int main(void)
{
    for (int i = 0; i < M; i++) table[i] = 0;

    int keys[] = {42, 17, 52, 27, 99, 62};
    for (int i = 0; i < 6; i++)
        insert(keys[i]);
    printf("\n");
    return 0;
}
