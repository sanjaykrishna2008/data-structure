#include <stdio.h>
#include <limits.h>
#define V 5

int main(void) {
    /* Cable costs in lakh (0 = no edge) */
    int c[V][V] = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}
    };

    int key[V], parent[V], in[V] = {0};
    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        parent[i] = -1;
    }
    key[0] = 0;
    int total = 0;

    for (int k = 0; k < V; k++) {
        int u = -1;
        for (int v = 0; v < V; v++)
            if (!in[v] && (u == -1 || key[v] < key[u]))
                u = v;

        if (u == -1 || key[u] == INT_MAX) {
            printf("graph is disconnected\n");
            return 1;
        }
        in[u] = 1;
        total += key[u];

        for (int v = 0; v < V; v++)
            if (!in[v] && c[u][v] > 0 && c[u][v] < key[v]) {
                key[v] = c[u][v];
                parent[v] = u;
            }
    }

    for (int i = 1; i < V; i++)
        printf("%d-%d(%d) ", parent[i], i, key[i]);
    printf("\ntotal cost = %d\n", total);
    return 0;
}
