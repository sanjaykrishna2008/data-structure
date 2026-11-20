#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int h;
    struct Node *l, *r;
} Node;

int ht(Node *n) { return n ? n->h : 0; }
int max2(int a, int b) { return a > b ? a : b; }
void fix(Node *n) { n->h = 1 + max2(ht(n->l), ht(n->r)); }
int balance(Node *n) { return n ? ht(n->l) - ht(n->r) : 0; }

Node *rotate_right(Node *y) {
    Node *x = y->l;
    Node *t2 = x->r;
    x->r = y;
    y->l = t2;
    fix(y);
    fix(x);
    return x;
}

Node *rotate_left(Node *x) {
    Node *y = x->r;
    Node *t2 = y->l;
    y->l = x;
    x->r = t2;
    fix(x);
    fix(y);
    return y;
}

Node *insert(Node *root, int key) {
    if (!root) {
        Node *n = malloc(sizeof *n);
        n->key = key;
        n->h = 1;
        n->l = n->r = NULL;
        return n;
    }
    if (key < root->key)
        root->l = insert(root->l, key);
    else if (key > root->key)
        root->r = insert(root->r, key);
    else
        return root; /* ignore duplicate */

    fix(root);
    int bf = balance(root);

    /* LL */
    if (bf > 1 && key < root->l->key)
        return rotate_right(root);
    /* RR */
    if (bf < -1 && key > root->r->key)
        return rotate_left(root);
    /* LR */
    if (bf > 1 && key > root->l->key) {
        root->l = rotate_left(root->l);
        return rotate_right(root);
    }
    /* RL */
    if (bf < -1 && key < root->r->key) {
        root->r = rotate_right(root->r);
        return rotate_left(root);
    }
    return root;
}

void preorder(Node *r) {
    if (!r) return;
    printf("%d ", r->key);
    preorder(r->l);
    preorder(r->r);
}

void destroy(Node *r) {
    if (!r) return;
    destroy(r->l);
    destroy(r->r);
    free(r);
}

int main(void) {
    int keys[] = {10, 20, 30, 40, 50, 25};
    Node *root = NULL;
    for (int i = 0; i < 6; i++)
        root = insert(root, keys[i]);

    printf("Preorder: ");
    preorder(root);
    printf("\nheight = %d\n", ht(root));

    destroy(root);
    return 0;
}
