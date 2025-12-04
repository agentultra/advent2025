#include <inttypes.h>
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

void print_grid(const struct grid* g) {
    for (uint64_t j = 0; j < g->h; ++j) {
        for (uint64_t i = 0; i < g->w; ++i) {
            printf("%c", *chr_vec_get(g->entries, (g->w * j) + i));
            if (i == g->w-1) printf("\n");
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

    chr_vec_free(grid_entries);
    return 0;
}
