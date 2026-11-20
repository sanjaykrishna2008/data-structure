#include <stdio.h>
#include <limits.h>
#define V 6
#define INF INT_MAX

int main(void) {
    /* Example non-negative graph from the report style */
    int w[V][V] = {
        {0, 7, 9, INF, INF, 14},
        {7, 0, 10, 15, INF, INF},
        {9, 10, 0, 11, INF, 2},
        {INF, 15, 11, 0, 6, INF},
        {INF, INF, INF, 6, 0, 9},
        {14, INF, 2, INF, 9, 0}
    };

    int dist[V], done[V] = {0}, prev[V];
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[0] = 0;

    for (int count = 0; count < V; count++) {
        int u = -1;
        for (int i = 0; i < V; i++)
            if (!done[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        if (u == -1 || dist[u] == INF) break;
        done[u] = 1;

        for (int v = 0; v < V; v++) {
            if (w[u][v] > 0 && !done[v]) {
                if (dist[u] != INF && dist[u] <= INT_MAX - w[u][v] &&
                    dist[u] + w[u][v] < dist[v]) {
                    dist[v] = dist[u] + w[u][v];
                    prev[v] = u;
                }
            }
        }
    }

    printf("Distances from 0: ");
    for (int i = 0; i < V; i++)
        if (dist[i] == INF) printf("INF ");
        else printf("%d ", dist[i]);
    printf("\n");
    return 0;
}
