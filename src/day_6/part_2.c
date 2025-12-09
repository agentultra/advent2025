#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vec.h>

#define BUF_SIZE 4096
#define VEC_SIZE 4096

struct grid {
    uint64_t w;
    uint64_t h;
    struct chr_vec_t* entries;
};

char* grid_at(const struct grid* g, int x, int y) {
    int mod_x = x < 0 ? x + g->w : x % g->w;
    int mod_y = y < 0 ? y + g->h : y % g->h;

    int ix = (g->w * mod_y) + mod_x;
    return chr_vec_get(g->entries, ix);
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
    struct chr_vec_t* grid_entries = chr_vec_new(VEC_SIZE);
    struct grid g = { .w = 0, .h = 0, .entries = grid_entries };

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0';
        buf_len--;

        if (g.w != buf_len) {
            g.w = buf_len;
        }

        for (int i = 0; i < buf_len; ++i) {
            chr_vec_insert(grid_entries, buf[i]);
        }

        g.h++;
    }

    uint64_t problem_total = 0;
    uint64_t grand_total = 0;

    for (int i = 0; i < g.w; ++i) {
        char digits[4] = {0};
        char* op_sym = grid_at(&g, i, 4);

        if (*op_sym == '+') {
            op = &add;
            problem_total = 0;
        } else if (*op_sym == '*') {
            op = &mul;
            problem_total = 1;
        }

        for (int j = 0; j < g.h - 1; ++j) {
            char* c = grid_at(&g, i, j);
            digits[j] = *c;
        }

        int num = atoi(digits);

        if (num == 0) {
            grand_total += problem_total;
            problem_total = 0;
            continue;
        } else {
            problem_total = op(problem_total, num);
        }
    }

    grand_total += problem_total;

    printf("Part 2: %" PRIu64 "\n", grand_total);
    return 0;
}
