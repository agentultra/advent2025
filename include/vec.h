#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <inttypes.h>
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

    if (new_entries == NULL) {
        printf("Out of memory: unable to copy vector\n.");
        exit(-1);
    }

    if(memcpy(new_entries, vec->entries, vec->capacity) != NULL) {
        struct chr_vec_t* new_vec =
            (struct chr_vec_t*)malloc(sizeof (struct chr_vec_t));

        new_vec->ix = vec->ix;
        new_vec->capacity = vec->capacity;
        new_vec->entries = new_entries;

        return new_vec;
    }

    return NULL;
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

struct vec_t {
    size_t ix;
    size_t elem_size;
    uint64_t capacity;
    void* entries;
};

struct vec_t* vec_new(int capacity, size_t elem_size) {
    void* entries = (void*)malloc(capacity * elem_size);

    if (!entries) {
        printf("Error allocating char vector entries\n");
        exit(-1);
    }

    struct vec_t* vector =
        (struct vec_t*)malloc(sizeof (struct vec_t));

    if (!vector) {
        printf("Error allocating vector\n");
        exit(-1);
    }

    vector->ix = 0;
    vector->elem_size = elem_size;
    vector->capacity = capacity;
    vector->entries = entries;

    return vector;
}

void vec_free(struct vec_t* vec) {
    free(vec->entries);
    free(vec);
}

struct vec_t* vec_copy(struct vec_t* vec) {
    void* entries = (void*)malloc(vec->capacity * vec->elem_size);

    memcpy(entries, vec->entries, vec->ix * vec->elem_size);

    struct vec_t* cpy = (struct vec_t*)malloc(sizeof (struct vec_t));
    cpy->ix = vec->ix;
    cpy->elem_size = vec->elem_size;
    cpy->capacity = vec->capacity;
    cpy->entries = entries;

    return cpy;
}

uint64_t vec_size(const struct vec_t* vec) {
    return vec->ix;
}

void vec_insert(struct vec_t* vec, void* elem) {
    assert(vec != NULL);
    assert(elem != NULL);

    // resize
    if (vec->ix == vec->capacity) {
        size_t new_capacity = vec->capacity * RESIZE_FACTOR;
        void* new_entries = (void*)malloc(new_capacity * vec->elem_size);

        if (new_entries == NULL) {
            printf("Unable to resize vec_t: out of memory.\n");
            exit(-1);
        }

        memcpy(new_entries, vec->entries, vec->ix * vec->elem_size);

        void* old_entries = vec->entries;
        vec->entries = new_entries;

        free(old_entries);
    }

    void* offset = vec->entries + (vec->ix * vec->elem_size);
    memcpy(offset, elem, vec->elem_size);
    vec->ix++;
}

void* vec_get(const struct vec_t* vec, size_t i) {
    assert(vec != NULL);

    if (i > vec->ix) {
        return NULL;
    }

    size_t offset = i * vec->elem_size;
    return vec->entries + offset;
}

void vec_set(struct vec_t* vec, size_t i, void* elem) {
    assert(vec != NULL);
    assert(i <= vec->ix);

    void* offset = vec->entries + (i * vec->elem_size);
    memcpy(offset, elem, vec->elem_size);
}

void* vec_peek(struct vec_t* vec) {
    size_t offset = vec->ix * vec->elem_size;

    return vec->entries + offset;
}

#endif
