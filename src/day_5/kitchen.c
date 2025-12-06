#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

#define BUF_SIZE 400
#define INIT_INTERVAL_VEC_SIZE 100

#define MIN(a, b) (a <= b ? a : b)
#define MAX(a, b) (a >= b ? a : b)

struct interval {
    uint64_t lo;
    uint64_t hi;
};

uint64_t interval_size(const struct interval* i) {
    assert(i->hi >= i->lo);

    return 1 + (i->hi - i->lo);
}

bool interval_overlaps(const struct interval* a, const struct interval* b) {
    return (a->lo <= b->lo && a->hi >= b->lo)
        || (b->lo <= a->lo && b->hi >= a->lo)
        || (a->lo == b->lo && a->hi == b->hi);
}

// if a overlaps b, merge b into a.
void interval_merge(struct interval* a, const struct interval* b) {
    if (!interval_overlaps(a, b)) {
        return;
    }

    a->lo = MIN(a->lo, b->lo);
    a->hi = MAX(a->hi, b->hi);
}

// an interval tree node
struct node {
    uint64_t max; // the greatest hi value of child intervals
    struct interval* interval;
    struct node* left;
    struct node* right;
};

void free_node(struct node* n) {
    free(n->interval);

    if (n->left != NULL) {
        free_node(n->left);
    }

    if (n->right != NULL) {
        free_node(n->right);
    }

    free(n);
}

struct node* node_new(struct interval i) {
    struct interval* ni = (struct interval*)malloc(sizeof (struct interval));

    if (ni == NULL) {
        printf("Error allocating new node interval.\n");
        exit(-1);
    }

    ni->lo = i.lo;
    ni->hi = i.hi;

    struct node* n = (struct node*)malloc(sizeof (struct node));

    if (n == NULL) {
        printf("Error allocating new node.\n");
        exit(-1);
    }

    n->max = i.hi;
    n->interval = ni;
    n->left = NULL;
    n->right = NULL;

    return n;
}

void node_insert(struct node** root, struct interval i) {
    if (*root == NULL) {
        *root = node_new(i);
        return;
    }

    uint64_t root_lo = (*root)->interval->lo;

    if (i.lo < root_lo) {
        node_insert(&(*root)->left, i);
     } else {
        node_insert(&(*root)->right, i);
     }

    if ((*root)->max < i.hi) {
        (*root)->max = i.hi;
    }

    return;
}

bool contains(const struct node* root, uint64_t v) {
    if (root == NULL) {
        return false;
    }

    if (root->interval->lo <= v && v <= root->interval->hi) {
        return true;
    }

    if (root->left != NULL && root->left->max >= v) {
        return contains(root->left, v);
    }

    return contains(root->right, v);
}

void in_order(const struct node* root, struct vec_t* intervals) {
    if (root == NULL) {
        return;
    }

    in_order(root->left, intervals);

    vec_insert(intervals, (void*)root->interval);

    in_order(root->right, intervals);
}

int main()
{
    char buf[BUF_SIZE];
    bool parsing_intervals = true;
    struct node* root = NULL;
    struct vec_t* intervals =
        vec_new(INIT_INTERVAL_VEC_SIZE, sizeof (struct interval));
    uint64_t sum_1 = 0;
    uint64_t sum_2 = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0';
        buf_len--;

        if (parsing_intervals) {
            uint64_t lo = 0;
            uint64_t hi = 0;
            if (sscanf(buf, "%lu-%lu", &lo, &hi) != EOF) {
                struct interval* i =
                    (struct interval*)malloc(sizeof (struct interval));
                i->lo = lo;
                i->hi = hi;

                node_insert(&root, (struct interval)*i);
            } else {
                // part 2
                in_order(root, intervals);
                assert(vec_size(intervals) > 0);

                struct interval* acc_interval = vec_get(intervals, 0);
                struct vec_t* merged = vec_new(INIT_INTERVAL_VEC_SIZE, sizeof (struct interval));
                for (int i = 1; i < vec_size(intervals); ++i) {
                    struct interval* next_interval = vec_get(intervals, i);

                    if (interval_overlaps(acc_interval, next_interval)) {
                        interval_merge(acc_interval, next_interval);
                    } else {
                        vec_insert(merged, acc_interval);
                        acc_interval = next_interval;
                    }
                }

                if (interval_overlaps(acc_interval, vec_peek(merged))) {
                    interval_merge(vec_peek(merged), acc_interval);
                } else {
                    vec_insert(merged, acc_interval);
                }

                for (int i = 0; i < vec_size(merged); ++i) {
                    sum_2 += interval_size(vec_get(merged, i));
                }

                parsing_intervals = false;
            }
        } else {
            uint64_t ingredient = 0;
            if (sscanf(buf, "%lu", &ingredient) != EOF) {
                if (contains(root, ingredient)) {
                    sum_1++;
                }
            }
        }
    }

    printf("Part 1: %" PRIu64 "\n", sum_1);
    printf("Part 2: %" PRIu64 "\n", sum_2);
    return 0;
}
