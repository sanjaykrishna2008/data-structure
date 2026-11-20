#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    int acc;
    struct Book *left, *right;
} Book;

Book *insert(Book *root, int acc) {
    if (root == NULL) {
        Book *p = malloc(sizeof *p);
        if (!p) return NULL;
        p->acc = acc;
        p->left = p->right = NULL;
        return p;
    }
    if (acc < root->acc)
        root->left = insert(root->left, acc);
    else if (acc > root->acc)
        root->right = insert(root->right, acc);
    /* duplicates ignored */
    return root;
}

int height(const Book *root) {
    if (root == NULL) return -1;
    int lh = height(root->left);
    int rh = height(root->right);
    return 1 + (lh > rh ? lh : rh);
}

Book *delete_key(Book *r, int key) {
    if (!r) return NULL;
    if (key < r->acc) r->left = delete_key(r->left, key);
    else if (key > r->acc) r->right = delete_key(r->right, key);
    else {
        if (!r->left || !r->right) {
            Book *child = r->left ? r->left : r->right;
            free(r);
            return child;
        }
        Book *s = r->right;
        while (s->left) s = s->left;
        r->acc = s->acc;
        r->right = delete_key(r->right, s->acc);
    }
    return r;
}

void inorder(const Book *r) {
    if (!r) return;
    inorder(r->left);
    printf("%d ", r->acc);
    inorder(r->right);
}

void preorder(const Book *r) {
    if (!r) return;
    printf("%d ", r->acc);
    preorder(r->left);
    preorder(r->right);
}

void postorder(const Book *r) {
    if (!r) return;
    postorder(r->left);
    postorder(r->right);
    printf("%d ", r->acc);
}

void levelorder(const Book *root, size_t n) {
    if (!root) return;
    const Book **q = malloc(n * sizeof *q);
    if (!q) return;
    size_t front = 0, back = 0;
    q[back++] = root;
    while (front < back) {
        const Book *p = q[front++];
        printf("%d ", p->acc);
        if (p->left)  q[back++] = p->left;
        if (p->right) q[back++] = p->right;
    }
    free(q);
}

int search_cmp(const Book *root, int key, int *cmp) {
    *cmp = 0;
    while (root) {
        (*cmp)++;
        if (key == root->acc) return 1;
        root = (key < root->acc) ? root->left : root->right;
    }
    return 0;
}

void destroy(Book *r) {
    if (!r) return;
    destroy(r->left);
    destroy(r->right);
    free(r);
}

int main(void) {
    /* ---------- Experiment with 8 keys ---------- */
    int keys1[] = {500, 300, 700, 200, 400, 600, 800, 350};
    Book *root1 = NULL;
    for (int i = 0; i < 8; i++)
        root1 = insert(root1, keys1[i]);

    printf("Inorder (random order insertion):\n");
    inorder(root1);
    printf("\nheight = %d\n", height(root1));

    int cmp;
    search_cmp(root1, 350, &cmp);
    printf("comparisons for 350: %d\n", cmp);

    /* sorted insertion */
    Book *root2 = NULL;
    for (int i = 10; i <= 80; i += 10)
        root2 = insert(root2, i);
    printf("\nheight (sorted): %d\n", height(root2));
    search_cmp(root2, 80, &cmp);
    printf("comparisons for 80: %d\n", cmp);

    /* deletion experiment */
    int del_keys[] = {50, 30, 70, 20, 40, 60, 80, 65};
    Book *root3 = NULL;
    for (int i = 0; i < 8; i++)
        root3 = insert(root3, del_keys[i]);

    root3 = delete_key(root3, 20);
    root3 = delete_key(root3, 30);
    root3 = delete_key(root3, 50);

    printf("\nPreorder after deletions 20,30,50:\n");
    preorder(root3);
    printf("\n");

    destroy(root1);
    destroy(root2);
    destroy(root3);
    return 0;
}
