#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Trie {
    struct Trie *child[26];
    int words;   /* number of words passing through this node */
    int end;     /* 1 if a complete word ends here */
} Trie;

Trie *node(void) {
    Trie *t = calloc(1, sizeof *t);
    return t;
}

void insert(Trie *t, const char *w) {
    Trie *path[256];
    size_t len = 0;
    path[0] = t;
    for (size_t i = 0; w[i]; ++i) {
        if (w[i] < 'a' || w[i] > 'z' || len >= 254) return;
        int k = w[i] - 'a';
        if (!t->child[k]) t->child[k] = node();
        if (!t->child[k]) return;
        t = t->child[k];
        path[++len] = t;
    }
    if (!t->end) {
        t->end = 1;
        for (size_t i = 0; i <= len; ++i) path[i]->words++;
    }
}

int count_prefix(const Trie *t, const char *p) {
    for (size_t i = 0; p[i]; ++i) {
        if (p[i] < 'a' || p[i] > 'z') return 0;
        t = t->child[p[i] - 'a'];
        if (!t) return 0;
    }
    return t->words;
}

int trie_search(const Trie *t, const char *w) {
    for (size_t i = 0; w[i]; ++i) {
        if (w[i] < 'a' || w[i] > 'z') return 0;
        t = t->child[w[i] - 'a'];
        if (!t) return 0;
    }
    return t->end != 0;
}

void autocomplete_dfs(const Trie *t, char *buf, size_t depth) {
    if (!t) return;
    if (t->end) {
        buf[depth] = '\0';
        puts(buf);
    }
    for (int i = 0; i < 26; ++i) {
        if (t->child[i]) {
            buf[depth] = (char)('a' + i);
            autocomplete_dfs(t->child[i], buf, depth + 1);
        }
    }
}

static int has_child(const Trie *t) {
    for (int i = 0; i < 26; ++i)
        if (t->child[i]) return 1;
    return 0;
}

int delete_word(Trie *root, const char *w) {
    Trie *path[256];
    int edge[255];
    size_t len = 0;
    Trie *p = root;
    path[0] = root;
    for (size_t i = 0; w[i]; ++i) {
        if (w[i] < 'a' || w[i] > 'z' || len >= 254) return 0;
        int k = w[i] - 'a';
        if (!p->child[k]) return 0;
        edge[len] = k;
        p = p->child[k];
        path[++len] = p;
    }
    if (!p->end) return 0;
    p->end = 0;
    for (size_t i = 0; i <= len; ++i) path[i]->words--;
    for (size_t i = len; i > 0; --i) {
        if (path[i]->words == 0 && !has_child(path[i])) {
            free(path[i]);
            path[i - 1]->child[edge[i - 1]] = NULL;
        } else break;
    }
    return 1;
}

int main(void) {
    Trie *root = node();
    const char *contacts[] = {
        "anand", "anitha", "anu", "arun", "bala", "anusuya"
    };
    for (int i = 0; i < 6; i++)
        insert(root, contacts[i]);

    printf("a:%d an:%d anu:%d b:%d z:%d\n",
           count_prefix(root, "a"),
           count_prefix(root, "an"),
           count_prefix(root, "anu"),
           count_prefix(root, "b"),
           count_prefix(root, "z"));

    printf("\nAutocomplete for \"an\":\n");
    char buf[64] = "an";
    const Trie *p = root;
    for (int i = 0; buf[i]; i++)
        p = p->child[buf[i] - 'a'];
    autocomplete_dfs(p, buf, 2);

    return 0;
}
