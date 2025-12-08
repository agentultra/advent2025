#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

#define BUF_SIZE 5096
#define VEC_SIZE 1024

struct mat_t {
    uint64_t w;
    uint64_t h;
    struct vec_t* entries;
};

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

        char* token = strtok(buf, " ");

        if (mat.w == 0) w++;

        vec_insert(mat.entries, token);

        while ((token = strtok(NULL, " "))) {
            if (mat.w == 0) w++;

            vec_insert(mat.entries, token);
        }

        if (mat.w == 0) mat.w = w;

        mat.h++;
    }

    for (int i = 0; i < mat.w; ++i) {
        printf("Doing column %d\n", i);

        char* op_sym = mat_get(&mat, i, mat.h-1);
        uint64_t problem_total = 0;

        printf("Operator: %s\n", op_sym);

        if (strcmp(op_sym, "+") == 0) {
            op = &add;
            problem_total = 0;
        } else if (strcmp(op_sym, "*") == 0) {
            op = &mul;
            problem_total = 1;
        } else {
            printf("Unknown problem operator: %s\n", op_sym);
            exit(-1);
        }

        for (int j = 0; j < mat.h-1; ++j) {
            char* num_sym = mat_get(&mat, i, j);
            uint64_t num = strtoull(num_sym, NULL, 10);

            printf("Num: %" PRIu64 "\n", num);

            problem_total = op(problem_total, num);
        }

        printf("Total: %" PRIu64 "\n", problem_total);

        grand_total += problem_total;
    }

    printf("Part 1: %" PRIu64 "\n", grand_total);
    return 0;
}
