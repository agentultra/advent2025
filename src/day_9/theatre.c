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

struct rect_t {
    struct vec2_t* v1;
    struct vec2_t* v2;
    uint64_t area;
};

int by_area_desc(const void* a, const void* b) {
    struct rect_t* rect_a = (struct rect_t*)a;
    struct rect_t* rect_b = (struct rect_t*)b;

    if (rect_a->area <= rect_b->area) {
        return 1;
    } else if (rect_a->area == rect_b->area) {
        return 0;
    } else {
        return -1;
    }
}

bool edge_overlaps(const struct rect_t* r, const struct vec_t* points) {
    for (int i = 0; i < vec_size(points); ++i) {
        struct vec2_t* a = vec_get(points, i);
        struct vec2_t* b = vec_get(points, (i + 1) % vec_size(points));

        uint64_t min_x = MIN(a->x, b->x);
        uint64_t min_y = MIN(a->y, b->y);
        uint64_t max_x = MAX(a->x, b->x);
        uint64_t max_y = MAX(a->y, b->y);

        uint64_t r_min_x = MIN(r->v1->x, r->v2->x);
        uint64_t r_min_y = MIN(r->v1->y, r->v2->y);
        uint64_t r_max_x = MAX(r->v1->x, r->v2->x);
        uint64_t r_max_y = MAX(r->v1->y, r->v2->y);

        if (max_y < r_min_y + 1 || min_y > r_max_y - 1 || max_x < r_min_x + 1 || min_x > r_max_x - 1) {
            continue;
        }
        return true;
    }

    return false;
}

int main()
{
    char buf[BUF_SIZE];
    struct vec_t* points = vec_new(VEC_SIZE, sizeof (struct vec2_t));
    struct vec_t* rects = vec_new(VEC_SIZE, sizeof (struct rect_t));
    uint64_t max_x = 0;
    uint64_t max_y = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        struct vec2_t* v = (struct vec2_t*)malloc(sizeof (struct vec2_t));

        sscanf(buf, "%" PRIu64 ",%" PRIu64, &v->x, &v->y);

        if (v->x > max_x) max_x = v->x;
        if (v->y > max_y) max_y = v->y;

        vec_insert(points, v);
    }

    uint64_t max_area_1 = 0;

    for (int i = 0; i < vec_size(points); ++i) {
        struct vec2_t* v1 = vec_get(points, i);

        for (int j = 0; j < vec_size(points); ++j) {
            if (i == j) continue;

            struct vec2_t* v2 = vec_get(points, j);

            uint64_t a = area(v1, v2);

            struct rect_t* r = (struct rect_t*)malloc(sizeof (struct rect_t));
            r->v1 = v1;
            r->v2 = v2;
            r->area = a;

            vec_insert(rects, r);

            if (a > max_area_1) {
                max_area_1 = a;
            }
        }
    }

    vec_sort(rects, &by_area_desc);

    uint64_t max_area_2 = 0;

    for (int i = 0; i < vec_size(rects); ++i) {
        struct rect_t* r = vec_get(rects, i);

        if (!edge_overlaps(r, points)) {
            max_area_2 = r->area;
            break;
        }
    }

    printf("Part 1: %" PRIu64 "\n", max_area_1);
    printf("Part 2: %" PRIu64 "\n", max_area_2);
    return 0;
}
