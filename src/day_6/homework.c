#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

#define BUF_SIZE 1024
#define VEC_SIZE 1024

struct mat_t {
    uint64_t w;
    uint64_t h;
    struct vec_t* entries;
};

void rotate(struct mat_t* mat) {
    struct vec_t* rotated = vec_copy(mat->entries);

    for (int i = 0; i < mat->w * mat->h; ++i) {
        int orig_row = i / mat->w;
        int orig_col = i % mat->w;

        int new_row = orig_col;
        int new_col = mat->w - orig_row - 1;

        vec_set(rotated, new_row * mat->w + new_col, vec_get(mat->entries, i));
    }

    struct vec_t* old_entries = mat->entries;
    mat->entries = rotated;

    vec_free(old_entries);
}

char* mat_get(struct mat_t* mat, uint64_t x, uint64_t y) {
    return vec_get(mat->entries, (mat->w * y) + x);
}

uint64_t add(uint64_t a, uint64_t b) {
    return a + b;
}

uint64_t mul(uint64_t a, uint64_t b) {
    return a * b;
}

uint64_t (*op)(uint64_t, uint64_t);

int main()
{
    char buf[BUF_SIZE];
    struct vec_t* entries = vec_new(VEC_SIZE, sizeof (char*));
    struct mat_t mat = { .w = 0, .h = 0, .entries = entries };
    uint64_t w = 0;
    uint64_t grand_total = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0';
        buf_len--;
        printf("%s\n", buf);

        char* token = strtok(buf, " ");
        if (mat.w == 0) w++;
        printf("First: %s\n", token);
        vec_insert(mat.entries, token);

        while ((token = strtok(NULL, " "))) {
            if (mat.w == 0) w++;
            vec_insert(mat.entries, token);
        }

        if (mat.w == 0) mat.w = w;
        mat.h++;
    }

    assert(mat.w == mat.h);

    rotate(&mat);

    int row = 0;

    while (row < mat.h) {
        char* op_sym = vec_get(mat.entries, row * mat.h);
        uint64_t total = 0;

        if (strcmp(op_sym, "+") == 0) {
            op = &add;
        } else if (strcmp(op_sym, "*") == 0) {
            op = &mul;
            total = 1;
        } else {
            printf("Unknown operator: %s\n", op_sym);
            exit(-1);
        }

        for (int x = 1; x < mat.w; ++x) {
            char* num_sym = mat_get(&mat, x, row);
            uint64_t num = strtoll(num_sym, NULL, 10);

            total = op(total, num);
        }

        grand_total += total;

        row++;
    }

    printf("Part 1: %" PRIu64 "\n", grand_total);
    return 0;
}
