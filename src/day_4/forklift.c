#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"

#define BUFFER_SIZE 200
#define INIT_GRID_SIZE 800

struct grid {
    uint64_t w;
    uint64_t h;
    struct chr_vec_t* entries;
};

char* grid_at(const struct grid* g, int x, int y) {
    int ix = ((g->w * y) + x) % (g->w * g-> h);
    return chr_vec_get(g->entries, ix);
}

static int cardinals[8][2] = {
    {-1, -1}, {0, -1}, {1, -1},
    {-1,  0},          {1,  0},
    {-1,  1}, {0,  1}, {1,  1}
};

bool is_accessible(const struct grid* g, int x, int y) {
    int free_spaces = 0;

    for (size_t i = 0; i < sizeof (cardinals); ++i) {
        int cardinal_x = x + cardinals[i][0];
        int cardinal_y = y + cardinals[i][1];
        char* t = grid_at(g, cardinal_x, cardinal_y);

        if (*t == '.') free_spaces++;
    }

    return free_spaces >= 4;
}

void print_grid(const struct grid* g) {
    for (uint64_t y = 0; y < g->h; y++) {
        for (uint64_t x = 0; x < g->w; ++x) {
            char* t = grid_at(g, x, y);
            if (*t == '@' && is_accessible(g, x, y)) {
                printf("x");
            } else {
                printf("%c", *grid_at(g, x, y));
            }
            if (x == g->w-1) printf("\n");
        }
    }
}

int main()
{
    char buf[BUFFER_SIZE];
    struct chr_vec_t* grid_entries = chr_vec_new(INIT_GRID_SIZE);
    struct grid g = { .w = 0, .h = 0, .entries = grid_entries };

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0'; // strip newline
        buf_len--;

        if (g.w != buf_len) {
            g.w = buf_len;
        }

        for (int i = 0; i < buf_len; ++i) {
            chr_vec_insert(grid_entries, buf[i]);
        }

        g.h++;
    }

    printf("Grid W: %" PRIu64 ", Grid H: %" PRIu64 "\n", g.w, g.h);
    print_grid(&g);

    uint64_t sum = 0;

    for (int y = 0; y < g.h; ++y) {
        for (int x = 0; x < g.w; ++x) {
            char* t = grid_at(&g, x, y);

            if (*t == '@') {
                if (is_accessible(&g, x, y)) {
                    sum++;
                }
            }
        }
    }

    chr_vec_free(grid_entries);

    printf("Part 1: %" PRIu64 "\n", sum);
    return 0;
}
