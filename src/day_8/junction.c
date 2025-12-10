#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
#include "vec.h"

#define BUF_SIZE 1024
#define VEC_SIZE 1024
#define HASH_SIZE 1024
#define MAX_KEY_SIZE 100

struct vec3_t {
    uint64_t x;
    uint64_t y;
    uint64_t z;
};

bool vec3_equal(const struct vec3_t* a, const struct vec3_t* b) {
    return a->x == b->x && a->y == b->y && a->z == b->z;
}

void print_vec3(const struct vec3_t* v) {
    printf("(%" PRIu64 ", %" PRIu64 ", %" PRIu64")\n", v->x, v->y, v->z);
}

char* vec3_key(const struct vec3_t* v) {
    char* key = (char*)malloc(MAX_KEY_SIZE);

    snprintf(key, MAX_KEY_SIZE, "%" PRIu64 ",%" PRIu64 ",%" PRIu64,
             v->x, v->y, v->z);

    return key;
}

uint64_t euclidean_distance(const struct vec3_t* a, const struct vec3_t* b) {
    uint64_t dx = b->x - a->x;
    uint64_t dy = b->y - a->y;
    uint64_t dz = b->z - a->z;

    return dx * dx + dy * dy + dz * dz;
}

struct pair {
    struct vec3_t* left;
    struct vec3_t* right;
    double distance;
};

void print_pair(const struct pair* p) {
    struct vec3_t* left = p->left;
    struct vec3_t* right = p->right;

    printf("(%" PRIu64 ", %" PRIu64 ", %" PRIu64") - ", left->x, left->y, left->z);
    printf("%.2f - ", p->distance);
    printf("(%" PRIu64 ", %" PRIu64 ", %" PRIu64")\n", right->x, right->y, right->z);
}

bool pair_equal(const struct pair* a, const struct pair* b) {
    return vec3_equal(a->left, b->left) && vec3_equal(a->right, b->right);
}

int by_distance(const void* a, const void* b) {
    struct pair* pair_a = (struct pair*) a;
    struct pair* pair_b = (struct pair*) b;

    if (pair_a->distance <= pair_b->distance) {
        return -1;
    } else if (pair_a->distance == pair_b->distance) {
        return 0;
    } else {
        return 1;
    }
}

bool is_connected(const struct hash_t* circuits, const struct pair* p) {
    assert(circuits != NULL);
    assert(p != NULL);

    struct vec3_t* a = p->left;
    struct vec3_t* b = p->right;

    struct hash_t* a_circuit = hash_get(circuits, vec3_key(a));
    struct hash_t* b_circuit = hash_get(circuits, vec3_key(b));

    bool a_connected = hash_get(b_circuit, vec3_key(a));
    bool b_connected = hash_get(a_circuit, vec3_key(b));

    return a_connected && b_connected;
}

void connect(struct hash_t* circuits, const struct pair* p) {
    struct vec3_t* a = p->left;
    struct vec3_t* b = p->right;

    struct hash_t* a_circuit = hash_get(circuits, vec3_key(a));
    struct hash_t* b_circuit = hash_get(circuits, vec3_key(b));

    hash_set(a_circuit, vec3_key(b), (void*)true);
    hash_set(b_circuit, vec3_key(a), (void*)true);
}

int by_size(const void* a, const void*b) {
    struct vec_t* circuit_a = (struct vec_t*)a;
    struct vec_t* circuit_b = (struct vec_t*)b;

    if (vec_size(circuit_a) < vec_size(circuit_b)) {
        return -1;
    } else if (vec_size(circuit_a) == vec_size(circuit_b)) {
        return 0;
    } else {
        return 1;
    }
}

int main()
{
    char buf[BUF_SIZE] = {0};
    struct vec_t* boxes = vec_new(VEC_SIZE, sizeof (struct vec3_t));
    struct vec_t* pairs = vec_new(VEC_SIZE, sizeof (struct pair));
    // hash<char, hash<char, bool>>
    struct hash_t* circuits = hash_new(HASH_SIZE);

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0';
        buf_len--;

        struct vec3_t* v = (struct vec3_t*)malloc(sizeof (struct vec3_t));

        sscanf(buf, "%" PRIu64 ",%" PRIu64 ",%" PRIu64, &v->x, &v->y, &v->z);

        vec_insert(boxes, v);

        struct hash_t* circuit = hash_new(HASH_SIZE);
        hash_set(circuits, vec3_key(v), circuit);
    }

    for (int i = 0; i < vec_size(boxes); ++i) {
        for (int j = 0; j < vec_size(boxes); ++j) {
            if (i == j) continue;

            struct vec3_t* box_1 = vec_get(boxes, i);
            struct vec3_t* box_2 = vec_get(boxes, j);
            double dist = euclidean_distance(box_1, box_2);
            struct pair* p = (struct pair*)malloc(sizeof (struct pair));

            p->left = box_1;
            p->right = box_2;
            p->distance = dist;

            vec_insert(pairs, p);
        }
    }

    vec_sort(pairs, &by_distance);

    for (int i = 0; i < vec_size(pairs); ++i) {
        struct pair* p = vec_get(pairs, i);

        print_pair(p);

        if (!is_connected(circuits, p)) {
            connect(circuits, p);
        }
    }

    /* vec_sort(circuits, &by_size); */

    /* for (int i = 0; i < vec_size(circuits); ++i) { */
    /*     struct vec_t* circuit = vec_get(circuits, i); */

    /*     printf("Circuit of size: %" PRIu64 "\n", vec_size(circuit)); */
    /* } */

    return 0;
}
