#include <stdio.h>
#define V 6

char *task[] = {"Lease", "Power", "Racks", "Network", "Staff", "Launch"};

/* L=0 P=1 R=2 N=3 S=4 X=5
   Edges: L→P, L→R, L→S, P→N, R→N, N→X, S→X */
int g[V][V] = {
    {0,1,1,0,1,0}, /* Lease */
    {0,0,0,1,0,0}, /* Power */
    {0,0,0,1,0,0}, /* Racks */
    {0,0,0,0,0,1}, /* Network */
    {0,0,0,0,0,1}, /* Staff */
    {0,0,0,0,0,0}  /* Launch */
};

int main(void) {
    int indeg[V] = {0};
    for (int u = 0; u < V; u++)
        for (int v = 0; v < V; v++)
            if (g[u][v]) indeg[v]++;

    int queue[V], head = 0, tail = 0;
    for (int i = 0; i < V; i++)
        if (indeg[i] == 0) queue[tail++] = i;

    int done = 0;
    while (head < tail) {
        int u = queue[head++];
        printf("%s ", task[u]);
        done++;
        for (int v = 0; v < V; v++) {
            if (g[u][v]) {
                indeg[v]--;
                if (indeg[v] == 0)
                    queue[tail++] = v;
            }
        }
    }
    printf("\n");
    if (done == V) printf("valid schedule\n");
    else printf("cycle detected\n");
    return 0;
}
