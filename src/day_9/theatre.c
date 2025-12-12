#include <inttypes.h>
#include "math.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vec.h"

#define BUF_SIZE 512
#define VEC_SIZE 512

#define MAX(a, b) (a >= b ? a : b)
#define MIN(a, b) (a <= b ? a : b)

struct vec2_t {
    uint64_t x;
    uint64_t y;
};

void print_vec2(struct vec2_t* v) {
    printf("(%" PRIu64 ", %" PRIu64 ")", v->x, v->y);
}

uint64_t area(struct vec2_t* m, struct vec2_t* n) {
    uint64_t a = MAX(m->x, n->x) - MIN(m->x, n->x);
    uint64_t b = MAX(m->y, n->y) - MIN(m->y, n->y);

    return (a + 1) * (b + 1);
}

struct grid {
    uint64_t w;
    uint64_t h;
    struct vec_t* entries;
};

struct grid* grid_new(uint64_t w, uint64_t h, void* default_elem, size_t elem_size) {
    size_t entries_size = w * h;
    struct vec_t* entries = vec_new(entries_size, elem_size);

    for (size_t i = 0; i < entries_size; ++i) {
        vec_insert(entries, default_elem);
    }

    struct grid* g = (struct grid*)malloc(sizeof (struct grid));

    if (g == NULL) {
        printf("Unable to allocation new grid: out of memory.\n");
        exit(-1);
    }

    g->w = w;
    g->h = h;
    g->entries = entries;

    return g;
}

void grid_set(struct grid* g, int x, int y, void* elem) {
    int mod_x = x < 0 ? x + g->w : x % g->w;
    int mod_y = y < 0 ? y + g->h : y % g->h;

    int ix = (g->w * mod_y) + mod_x;
    vec_set(g->entries, ix, elem);
}

enum tile* grid_at(const struct grid* g, int x, int y) {
    int mod_x = x < 0 ? x + g->w : x % g->w;
    int mod_y = y < 0 ? y + g->h : y % g->h;

    int ix = (g->w * mod_y) + mod_x;
    enum tile* t = vec_get(g->entries, ix);
    return t;
}

enum tile {
    EMPTY,
    GREEN,
    RED
};

void print_grid(struct grid* g) {
    for (int j = 0; j < g->h; ++j) {
        for (int i = 0; i < g->w; ++i) {
            enum tile* t = grid_at(g, i, j);
            switch (*t) {
            case EMPTY: {
                printf(".");
                break;
            }
            case GREEN: {
                printf("X");
                break;
            }
            case RED: {
                printf("#");
                break;
            }
            default:
                printf("?");
                break;
            }

            if (i == g->w-1) printf("\n");
        }
    }
}

static enum tile empty = EMPTY;
static enum tile red = RED;

int main()
{
    char buf[BUF_SIZE];
    struct vec_t* points = vec_new(VEC_SIZE, sizeof (struct vec2_t));
    uint64_t max_x = 0;
    uint64_t max_y = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        struct vec2_t* v = (struct vec2_t*)malloc(sizeof (struct vec2_t));

        sscanf(buf, "%" PRIu64 ",%" PRIu64, &v->x, &v->y);

        if (v->x > max_x) max_x = v->x;
        if (v->y > max_y) max_y = v->y;

        vec_insert(points, v);
    }

    uint64_t max_area = 0;
    struct grid* map = grid_new(max_x + 2, max_y + 2, &empty, sizeof (enum tile));

    for (int i = 0; i < vec_size(points); ++i) {
        struct vec2_t* v1 = vec_get(points, i);

        grid_set(map, v1->x, v1->y, &red);

        for (int j = 0; j < vec_size(points); ++j) {
            if (i == j) continue;

            struct vec2_t* v2 = vec_get(points, j);

            uint64_t a = area(v1, v2);

            if (a > max_area) {
                max_area = a;
            }
        }
    }

    print_grid(map);

    printf("Part 1: %" PRIu64 "\n", max_area);
    return 0;
}
