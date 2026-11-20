#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint32_t hash_poly(const char *s, uint32_t table_size)
{
    uint32_t h = 0;
    while (*s) {
        h = h * 31u + (unsigned char)*s;
        ++s;
    }
    return h % table_size;
}

uint32_t hash_fnv1a(const char *s, uint32_t table_size)
{
    uint32_t h = 2166136261u;
    while (*s) {
        h ^= (unsigned char)*s;
        h *= 16777619u;
        ++s;
    }
    return h % table_size;
}

/* Original bad hash */
unsigned bad_hash(const char *reg)
{
    return (reg[0] + reg[1]) % 1000;
}

int main(void)
{
    const char *regs[] = {
        "TN09AB1234", "TN10CD5678", "KA05XY9999",
        "MH12PQ3456", "TN22ZZ0001"
    };
    int n = 5;
    uint32_t M = 131071;

    printf("Bad hash (first two chars):\n");
    for (int i = 0; i < n; i++)
        printf("  %s -> %u\n", regs[i], bad_hash(regs[i]));

    printf("\nPolynomial hash (table size %u):\n", M);
    for (int i = 0; i < n; i++)
        printf("  %s -> %u\n", regs[i], hash_poly(regs[i], M));

    printf("\nFNV-1a hash:\n");
    for (int i = 0; i < n; i++)
        printf("  %s -> %u\n", regs[i], hash_fnv1a(regs[i], M));

    return 0;
}
