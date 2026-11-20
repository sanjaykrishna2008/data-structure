#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct Order {
    char id[16];
    char restaurant[24];
    int amount;
    long long promised_at;
    unsigned long long arrival_seq;
    int status;              /* 0=pending, 1=assigned, 2=cancelled */
    size_t heap_index;
    struct Order *hash_next;
} Order;

typedef struct {
    Order **buckets;
    size_t bucket_count, hash_size;
    Order **heap;
    size_t heap_size, heap_cap;
} Platform;

uint32_t fnv1a(const char *s)
{
    uint32_t h = 2166136261u;
    while (*s) { h ^= (unsigned char)*s++; h *= 16777619u; }
    return h;
}

Order *find_order(Platform *p, const char *id)
{
    size_t b = fnv1a(id) % p->bucket_count;
    for (Order *o = p->buckets[b]; o; o = o->hash_next)
        if (strcmp(o->id, id) == 0) return o;
    return NULL;
}

int higher(const Order *a, const Order *b)
{
    if (a->promised_at != b->promised_at)
        return a->promised_at < b->promised_at;
    return a->arrival_seq < b->arrival_seq;
}

void heap_swap(Platform *p, size_t i, size_t j)
{
    Order *t = p->heap[i];
    p->heap[i] = p->heap[j];
    p->heap[j] = t;
    p->heap[i]->heap_index = i;
    p->heap[j]->heap_index = j;
}

void sift_up(Platform *p, size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (!higher(p->heap[i], p->heap[parent])) break;
        heap_swap(p, i, parent);
        i = parent;
    }
}

void sift_down(Platform *p, size_t i)
{
    for (;;) {
        size_t l = 2*i + 1, r = l + 1, best = i;
        if (l < p->heap_size && higher(p->heap[l], p->heap[best])) best = l;
        if (r < p->heap_size && higher(p->heap[r], p->heap[best])) best = r;
        if (best == i) break;
        heap_swap(p, i, best);
        i = best;
    }
}

void hash_insert(Platform *p, Order *o)
{
    size_t b = fnv1a(o->id) % p->bucket_count;
    o->hash_next = p->buckets[b];
    p->buckets[b] = o;
    ++p->hash_size;
}

void hash_remove(Platform *p, Order *target)
{
    size_t b = fnv1a(target->id) % p->bucket_count;
    Order **cur = &p->buckets[b];
    while (*cur) {
        if (*cur == target) {
            *cur = target->hash_next;
            target->hash_next = NULL;
            --p->hash_size;
            return;
        }
        cur = &(*cur)->hash_next;
    }
}

void heap_push(Platform *p, Order *o)
{
    if (p->heap_size == p->heap_cap) {
        p->heap_cap *= 2;
        p->heap = realloc(p->heap, p->heap_cap * sizeof(Order*));
    }
    size_t i = p->heap_size++;
    p->heap[i] = o;
    o->heap_index = i;
    sift_up(p, i);
}

Order *heap_remove_at(Platform *p, size_t i)
{
    if (i >= p->heap_size) return NULL;
    Order *out = p->heap[i];
    --p->heap_size;
    if (i < p->heap_size) {
        p->heap[i] = p->heap[p->heap_size];
        p->heap[i]->heap_index = i;
        if (i && higher(p->heap[i], p->heap[(i-1)/2])) sift_up(p, i);
        else sift_down(p, i);
    }
    out->heap_index = (size_t)-1;
    return out;
}

int place_order(Platform *p, Order *o)
{
    if (find_order(p, o->id)) return 0;
    hash_insert(p, o);
    heap_push(p, o);
    o->status = 0;
    return 1;
}

Order *assign_next_rider(Platform *p, long long now)
{
    if (p->heap_size == 0 || p->heap[0]->promised_at >= now) return NULL;
    Order *o = heap_remove_at(p, 0);
    o->status = 1;
    return o;
}

int cancel_order(Platform *p, const char *id)
{
    Order *o = find_order(p, id);
    if (!o || o->status != 0) return 0;
    heap_remove_at(p, o->heap_index);
    hash_remove(p, o);
    o->status = 2;
    return 1;
}

void init_platform(Platform *p, size_t buckets)
{
    p->bucket_count = buckets;
    p->buckets = calloc(buckets, sizeof(Order*));
    p->hash_size = 0;
    p->heap_cap = 64;
    p->heap = malloc(p->heap_cap * sizeof(Order*));
    p->heap_size = 0;
}

int main(void)
{
    Platform plat;
    init_platform(&plat, 17);

    Order orders[5];
    strcpy(orders[0].id, "O100"); strcpy(orders[0].restaurant, "R1");
    orders[0].amount = 250; orders[0].promised_at = 10; orders[0].arrival_seq = 1;

    strcpy(orders[1].id, "O101"); strcpy(orders[1].restaurant, "R2");
    orders[1].amount = 180; orders[1].promised_at = 7; orders[1].arrival_seq = 2;

    strcpy(orders[2].id, "O102"); strcpy(orders[2].restaurant, "R1");
    orders[2].amount = 300; orders[2].promised_at = 12; orders[2].arrival_seq = 3;

    for (int i = 0; i < 3; i++)
        place_order(&plat, &orders[i]);

    printf("Pending after place: %zu\n", plat.heap_size);

    Order *a = assign_next_rider(&plat, 15);
    if (a) printf("Assigned: %s (promised %lld)\n", a->id, a->promised_at);

    cancel_order(&plat, "O102");
    printf("After cancel O102, pending: %zu\n", plat.heap_size);

    a = assign_next_rider(&plat, 15);
    if (a) printf("Assigned: %s\n", a->id);
    else printf("No more overdue\n");

    free(plat.buckets);
    free(plat.heap);
    return 0;
}
