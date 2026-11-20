#include <stdio.h>
#include <stdlib.h>

#define T 2
#define MAX_KEYS (2*T - 1)
#define MAX_CHILD (2*T)

typedef struct BNode {
    int n, leaf;
    int key[MAX_KEYS];
    struct BNode *child[MAX_CHILD];
} BNode;

BNode *new_node(int leaf) {
    BNode *x = calloc(1, sizeof *x);
    x->leaf = leaf;
    return x;
}

void split_child(BNode *x, int i) {
    BNode *y = x->child[i];
    BNode *z = new_node(y->leaf);
    z->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        z->key[j] = y->key[j + T];
    if (!y->leaf)
        for (int j = 0; j < T; j++)
            z->child[j] = y->child[j + T];

    y->n = T - 1;

    for (int j = x->n; j >= i + 1; j--)
        x->child[j + 1] = x->child[j];
    x->child[i + 1] = z;

    for (int j = x->n - 1; j >= i; j--)
        x->key[j + 1] = x->key[j];
    x->key[i] = y->key[T - 1];
    x->n++;
}

void insert_nonfull(BNode *x, int k) {
    int i = x->n - 1;
    if (x->leaf) {
        while (i >= 0 && k < x->key[i]) {
            x->key[i + 1] = x->key[i];
            i--;
        }
        x->key[i + 1] = k;
        x->n++;
    } else {
        while (i >= 0 && k < x->key[i]) i--;
        i++;
        if (x->child[i]->n == MAX_KEYS) {
            split_child(x, i);
            if (k > x->key[i]) i++;
        }
        insert_nonfull(x->child[i], k);
    }
}

BNode *insert(BNode *root, int k) {
    if (root->n == MAX_KEYS) {
        BNode *s = new_node(0);
        s->child[0] = root;
        split_child(s, 0);
        insert_nonfull(s, k);
        return s;
    }
    insert_nonfull(root, k);
    return root;
}

int btree_search(BNode *x, int k) {
    if (!x) return 0;
    int i = 0;
    while (i < x->n && k > x->key[i]) i++;
    if (i < x->n && k == x->key[i]) return 1;
    if (x->leaf) return 0;
    return btree_search(x->child[i], k);
}

void print_tree(BNode *x, int level) {
    if (!x) return;
    printf("Level %d: ", level);
    for (int i = 0; i < x->n; i++)
        printf("%d ", x->key[i]);
    printf("\n");
    if (!x->leaf)
        for (int i = 0; i <= x->n; i++)
            print_tree(x->child[i], level + 1);
}

int main(void) {
    int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
    BNode *root = new_node(1);

    for (int i = 0; i < 8; i++) {
        root = insert(root, keys[i]);
        printf("After inserting %d:\n", keys[i]);
        print_tree(root, 0);
        printf("\n");
    }

    printf("Search 17: %s\n", btree_search(root, 17) ? "found" : "not found");
    printf("Search 18: %s\n", btree_search(root, 18) ? "found" : "not found");
    return 0;
}
