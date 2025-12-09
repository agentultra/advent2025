// Adapted from https://benhoyt.com/writings/hash-table-in-c/

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// See hash_key function
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

#define MAX(a, b) (a >= b ? a : b)

#define MIN_HASH_CAPACITY 20
// When expanding, capacity * HASH_EXPAND_FACTOR
#define HASH_EXPAND_FACTOR 2
// When table size crosses this threshold, capacity /
// HASH_THRESHOLD_FACTOR, expand
#define HASH_THRESHOLD_FACTOR 2

struct entry_t {
    char* key;
    void* data;
};

struct hash_t {
    size_t size;
    size_t capacity;
    struct entry_t* entries;
};

struct hash_t* hash_new(size_t capacity) {
    size_t table_capacity = MAX(MIN_HASH_CAPACITY, capacity);

    struct entry_t* entries =
        (struct entry_t*)calloc(table_capacity, sizeof (struct entry_t));

    if (entries == NULL) {
        printf("Unable to allocate hash table entries: out of memory\n");
        exit(-1);
    }

    struct hash_t* table = (struct hash_t*)malloc(sizeof (struct hash_t));

    if (table == NULL) {
        printf("Unable to allocate hash table: out of memory\n");
        exit(-1);
    }

    table->size = 0;
    table->capacity = table_capacity;
    table->entries = entries;

    return table;
}

void hash_free(struct hash_t* ht) {
    assert(ht != NULL);

    for (size_t i = 0; i < ht->capacity; ++i) {
        free((void*)ht->entries[i].key);
    }

    free(ht->entries);
    free(ht);
}

// Return 64-bit FNV-1a hash for key (NUL-terminated). See description:
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
//
// Not typically used much these days but good enough for AoC.
static uint64_t hash_key(const char* key) {
    uint64_t hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

void* hash_get(struct hash_t* ht, const char* key) {
    assert(ht != NULL);

    uint64_t hash = hash_key(key);
    size_t ix = (size_t)(hash & (uint64_t)ht->capacity - 1);

    while (ht->entries[ix].key != NULL) {
        if (strcmp(ht->entries[ix].key, key) == 0) {
            return ht->entries[ix].data;
        }

        ix++;

        if (ix >= ht->capacity) {
            ix = 0;
        }
    }

    return NULL;
}

void hash_set_entry(struct entry_t* entries, size_t capacity,
                    const char* key, void* data, size_t* length) {
    assert(entries != NULL);

    uint64_t hash = hash_key(key);
    size_t ix = (size_t)(hash & (uint64_t)capacity - 1);

    while (entries[ix].key != NULL) {
        if (strcmp(entries[ix].key, key) == 0) {
            entries[ix].data = data;
        }

        ix++;

        if (ix >= capacity) {
            ix = 0;
        }
    }

    if (length != NULL) {
        key = strdup(key);

        if (key == NULL) {
            printf("Error duplicating key: out of memory.\n");
            exit(-1);
        }

        (*length)++;
    }

    entries[ix].key = (char*)key;
    entries[ix].data = data;
}

void hash_set(struct hash_t* ht, const char* key, void* data) {
    assert(ht != NULL);
    assert(data != NULL);

    // check resize
    if (ht->size >= ht->capacity / HASH_THRESHOLD_FACTOR) {
        size_t new_capacity = ht->capacity * HASH_EXPAND_FACTOR;

        if (new_capacity < ht->capacity) {
            printf("Hash table capacity overflow error.\n");
            exit(-1);
        }

        struct entry_t* new_entries =
            (struct entry_t*)calloc(new_capacity, sizeof (struct entry_t));

        if (new_entries == NULL) {
            printf("Error resizing table: out of memory.\n");
            exit(-1);
        }

        for (size_t i = 0; i < ht->capacity; ++i) {
            struct entry_t entry = ht->entries[i];

            if (entry.key != NULL) {
                hash_set_entry(new_entries, new_capacity,
                               entry.key, entry.data, NULL);
            }
        }

        for (size_t i = 0; i < ht->capacity; ++i) {
            free((void*)ht->entries[i].key);
        }

        free(ht->entries);
        ht->entries =  new_entries;
        ht->capacity = new_capacity;
    }

    hash_set_entry(ht->entries, ht->capacity, key, data, &ht->size);
}
