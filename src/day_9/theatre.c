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

int main()
{
    char buf[BUF_SIZE];
    struct vec_t* points = vec_new(VEC_SIZE, sizeof (struct vec2_t));

    while (fgets(buf, sizeof (buf), stdin)) {
        struct vec2_t* v = (struct vec2_t*)malloc(sizeof (struct vec2_t));

        sscanf(buf, "%" PRIu64 ",%" PRIu64, &v->x, &v->y);

        vec_insert(points, v);
    }

    uint64_t max_area = 0;

    for (int i = 0; i < vec_size(points); ++i) {
        struct vec2_t* v1 = vec_get(points, i);

        for (int j = 0; j < vec_size(points); ++j) {
            if (i == j) continue;

            struct vec2_t* v2 = vec_get(points, j);

//            printf("Testing: "); print_vec2(v1); printf(" "); print_vec2(v2); printf("\n");
            uint64_t a = area(v1, v2);
//            printf("\tArea: %" PRIu64 "\n", a);

            if (a > max_area) {
                max_area = a;
            }
        }
    }

    printf("Part 1: %" PRIu64 "\n", max_area);
    return 0;
}
