#include <stdio.h>
#define V 6

int g[V][V] = {
    /* 0  1  2  3  4  5 */
    {0, 1, 1, 0, 0, 0}, /* 0 */
    {1, 0, 0, 1, 1, 0}, /* 1 */
    {1, 0, 0, 0, 1, 0}, /* 2 */
    {0, 1, 0, 0, 0, 1}, /* 3 */
    {0, 1, 1, 0, 0, 1}, /* 4 */
    {0, 0, 0, 1, 1, 0}  /* 5 */
};

int seen[V];

void dfs(int u) {
    seen[u] = 1;
    printf("%d ", u);
    for (int v = 0; v < V; v++)
        if (g[u][v] && !seen[v])
            dfs(v);
}

int main(void) {
    for (int i = 0; i < V; i++) seen[i] = 0;
    dfs(0);
    printf("\n");
    return 0;
}
