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

bool vec2_equal(const struct vec2_t* v1, const struct vec2_t* v2) {
    return v1->x == v2->x && v1->y == v2->y;
}

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

static enum tile empty = EMPTY;
static enum tile green = GREEN;
static enum tile red = RED;

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

void flood_fill(struct grid* g) {
    for (int j = 0; j < g->h; ++j) {
        for (int i = 0; i < g->w; ++i) {
            enum tile* t = grid_at(g, i, j);

            if (*t != empty) {
                for (int x = i+1; x < g->w-1; ++x) {
                    enum tile* next = grid_at(g, x, j);

                    if (*next == empty) {
                        grid_set(g, x, j, &green);
                    } else {
                        break;
                    }
                }
            }
        }
    }
}

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

    struct vec2_t* init_v2 = vec_get(points, 0);
    struct vec2_t initial_position = *init_v2;
    struct vec2_t last_position = initial_position;

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

        if (vec2_equal(v1, &initial_position)) {
            continue;
        } else {
            if (last_position.x == v1->x) {
                for (int j = MIN(last_position.y, v1->y)+1; j < MAX(last_position.y, v1->y); ++j) {
                    grid_set(map, last_position.x, j, &green);
                }
            } else if (last_position.y == v1->y) {
                for (int i = MIN(last_position.x, v1->x)+1; i < MAX(last_position.x, v1->x); ++i) {
                    grid_set(map, i, last_position.y, &green);
                }
            } else {
                printf("Something went wrong: (%" PRIu64 ", %" PRIu64 ")\n", v1->x, v1->y);
            }
            last_position = *v1;
        }
    }

    for (int j = MIN(initial_position.y, last_position.y)+1; j < MAX(initial_position.y, last_position.y); ++j) {
        grid_set(map, initial_position.x, j, &green);
    }

    for (int i = MIN(initial_position.x, last_position.x)+1; i < MAX(initial_position.x, last_position.x); ++i) {
        grid_set(map, i, initial_position.y, &green);
    }

    flood_fill(map);

    uint64_t max_area_2 = 0;

    for (int j = 0; j < vec_size(points); ++j) {
        struct vec2_t* v1 = vec_get(points, j);

        for (int i = 0; i < vec_size(points); ++i) {
            if (i == j) continue;

            struct vec2_t* v2 = vec_get(points, i);
            struct vec2_t v3 = { .x = v1->x, .y = v2->y };
            struct vec2_t v4 = { .x = v2->x, .y = v1->y };

            enum tile* tile_v3 = grid_at(map, v3.x, v3.y);
            enum tile* tile_v4 = grid_at(map, v4.x, v4.y);

            if (*tile_v3 != empty && *tile_v4 != empty) {
                uint64_t rect_area = area(v1, v2);

                if (rect_area > max_area_2) {
                    max_area_2 = rect_area;
                }
            }
        }
    }

    //print_grid(map);

    printf("Part 1: %" PRIu64 "\n", max_area);
    printf("Part 2: %" PRIu64 "\n", max_area_2);
    return 0;
}
