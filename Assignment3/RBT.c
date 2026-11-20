#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    int key;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct {
    RBNode *root;
    RBNode *nil;          /* sentinel */
} RBTree;

RBNode *new_node(RBTree *t, int key) {
    RBNode *n = malloc(sizeof *n);
    n->key = key;
    n->color = RED;
    n->left = n->right = n->parent = t->nil;
    return n;
}

void left_rotate(RBTree *t, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != t->nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == t->nil) t->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void right_rotate(RBTree *t, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != t->nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == t->nil) t->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void rb_insert_fixup(RBTree *t, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(t, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(t, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = BLACK;
}

void rb_insert(RBTree *t, int key) {
    RBNode *z = new_node(t, key);
    RBNode *y = t->nil;
    RBNode *x = t->root;
    while (x != t->nil) {
        y = x;
        if (z->key < x->key) x = x->left;
        else x = x->right;
    }
    z->parent = y;
    if (y == t->nil) t->root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;
    rb_insert_fixup(t, z);
}

void inorder(RBTree *t, RBNode *x) {
    if (x == t->nil) return;
    inorder(t, x->left);
    printf("%d%c ", x->key, x->color == RED ? 'R' : 'B');
    inorder(t, x->right);
}

int main(void) {
    RBTree t;
    t.nil = malloc(sizeof(RBNode));
    t.nil->color = BLACK;
    t.nil->left = t.nil->right = t.nil->parent = t.nil;
    t.root = t.nil;

    int keys[] = {10, 20, 30, 15, 25, 5};
    for (int i = 0; i < 6; i++)
        rb_insert(&t, keys[i]);

    printf("Inorder (key + color): ");
    inorder(&t, t.root);
    printf("\n");
    return 0;
}
