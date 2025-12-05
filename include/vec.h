#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESIZE_FACTOR 2

struct chr_vec_t {
    size_t ix;
    uint64_t capacity;
    char* entries;
};

struct chr_vec_t* chr_vec_new(int capacity) {
    char* entries = (char*)malloc(capacity);

    if (!entries) {
        printf("Error allocating char vector entries\n");
        exit(-1);
    }

    struct chr_vec_t* vector =
        (struct chr_vec_t*)malloc(sizeof (struct chr_vec_t));

    if (!vector) {
        printf("Error allocating vector\n");
        exit(-1);
    }

    vector->ix = 0;
    vector->capacity = capacity;
    vector->entries = entries;

    return vector;
}

void chr_vec_free(struct chr_vec_t* vec) {
    free(vec->entries);
    free(vec);
}

// Caller still has to free original
struct chr_vec_t* chr_vec_copy(const struct chr_vec_t* vec) {
    assert(vec != NULL);

    char* new_entries = (char*)malloc(vec->capacity);
    memcpy(new_entries, vec->entries, vec->capacity);

    struct chr_vec_t* new_vec =
        (struct chr_vec_t*)malloc(sizeof (struct chr_vec_t));

    new_vec->ix = vec->ix;
    new_vec->capacity = vec->capacity;
    new_vec->entries = new_entries;

    return new_vec;
}

void chr_vec_insert(struct chr_vec_t* vec, char c) {
    assert(vec != NULL);

    // resize
    if (vec->ix == vec->capacity - 1) {
        uint64_t new_capacity = vec->capacity * RESIZE_FACTOR;
        char* new_entries = realloc(vec->entries, new_capacity);

        if (new_entries == NULL) {
            printf("Unable to reallocate char vector entries\n");
            exit(-1);
        }

        vec->capacity = new_capacity;
        vec->entries = new_entries;
    }

    vec->entries[vec->ix] = c;
    vec->ix++;
}

bool chr_vec_set(struct chr_vec_t* vec, size_t i, char c) {
    assert(vec != NULL);

    if (i > vec->ix) {
        return false;
    }

    vec->entries[i] = c;

    return true;
}

char* chr_vec_get(struct chr_vec_t* vec, size_t i) {
    assert(vec != NULL);

    if (i > vec->ix) {
        return NULL;
    }

    return &vec->entries[i];
}

char* chr_vec_peek(struct chr_vec_t* vec) {
    assert(vec != NULL);

    return &vec->entries[vec->ix];
}
