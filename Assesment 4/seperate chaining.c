#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS 7

typedef struct Member {
    char id[12];
    char name[20];
    struct Member *next;
} Member;

Member *table[BUCKETS];

unsigned hash(const char *s)
{
    unsigned h = 0;
    while (*s) h = h * 31u + (unsigned char)*s++;
    return h % BUCKETS;
}

void put(const char *id, const char *name)
{
    Member *m = malloc(sizeof *m);
    if (!m) return;
    snprintf(m->id, sizeof m->id, "%s", id);
    snprintf(m->name, sizeof m->name, "%s", name);
    unsigned b = hash(id);
    m->next = table[b];
    table[b] = m;
}

const char *get(const char *id, int *probes)
{
    *probes = 0;
    unsigned b = hash(id);
    for (Member *m = table[b]; m; m = m->next) {
        ++(*probes);
        if (strcmp(m->id, id) == 0) return m->name;
    }
    return NULL;
}

int main(void)
{
    put("CSE1043", "Arun");
    put("CSE1044", "Rahul");
    put("BME4120", "Priya");
    put("CIV5009", "Sneha");
    put("EEE3011", "Vikram");
    put("MEC2201", "Ananya");

    /* print bucket lengths */
    for (int b = 0; b < BUCKETS; b++) {
        int len = 0;
        for (Member *m = table[b]; m; m = m->next) len++;
        printf("%d", len);
    }
    printf("\n");

    int probes;
    const char *name = get("CSE1044", &probes);
    if (name) printf("CSE1044 -> %s (%d probes)\n", name, probes);
    else printf("CSE1044 -> not found\n");

    name = get("CSE9999", &probes);
    if (name) printf("CSE9999 -> %s (%d probes)\n", name, probes);
    else printf("CSE9999 -> not found (%d probes)\n", probes);

    /* free */
    for (int b = 0; b < BUCKETS; b++) {
        Member *m = table[b];
        while (m) {
            Member *tmp = m;
            m = m->next;
            free(tmp);
        }
    }
    return 0;
}
