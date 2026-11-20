#include <stdio.h>
#include <stdlib.h>

typedef struct SplayNode {
    int key;
    struct SplayNode *left, *right, *parent;
} SplayNode;

void rotate_left(SplayNode **root, SplayNode *x) {
    SplayNode *y = x->right;
    if (!y) return;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) *root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotate_right(SplayNode **root, SplayNode *y) {
    SplayNode *x = y->left;
    if (!x) return;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) *root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void splay(SplayNode **root, SplayNode *x) {
    while (x && x->parent) {
        SplayNode *p = x->parent, *g = p->parent;
        if (!g) {
            if (x == p->left) rotate_right(root, p);
            else rotate_left(root, p);
        } else if (x == p->left && p == g->left) {
            rotate_right(root, g);
            rotate_right(root, p);
        } else if (x == p->right && p == g->right) {
            rotate_left(root, g);
            rotate_left(root, p);
        } else if (x == p->right && p == g->left) {
            rotate_left(root, p);
            rotate_right(root, g);
        } else {
            rotate_right(root, p);
            rotate_left(root, g);
        }
    }
}

SplayNode *insert(SplayNode *root, int key) {
    SplayNode *z = malloc(sizeof *z);
    z->key = key;
    z->left = z->right = z->parent = NULL;

    if (!root) return z;

    SplayNode *p = root, *parent = NULL;
    while (p) {
        parent = p;
        if (key < p->key) p = p->left;
        else if (key > p->key) p = p->right;
        else { free(z); return root; }
    }
    z->parent = parent;
    if (key < parent->key) parent->left = z;
    else parent->right = z;

    splay(&root, z);
    return root;
}

void preorder(SplayNode *r) {
    if (!r) return;
    printf("%d ", r->key);
    preorder(r->left);
    preorder(r->right);
}

int main(void) {
    SplayNode *root = NULL;
    int keys[] = {6, 3, 8, 2, 4, 9};
    for (int i = 0; i < 6; i++)
        root = insert(root, keys[i]);

    printf("After building tree (preorder):\n");
    preorder(root);
    printf("\n");

    /* Access 3 then 9 */
    /* (In a real program you would search and then splay) */
    return 0;
}
