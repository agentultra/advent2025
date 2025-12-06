#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 400

struct interval {
    uint64_t lo;
    uint64_t hi;
};

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

int main()
{
    char buf[BUF_SIZE];
    bool parsing_intervals = true;
    struct node* root = NULL;
    uint64_t sum_1 = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0';
        buf_len--;

        if (parsing_intervals) {
            uint64_t lo = 0;
            uint64_t hi = 0;
            if (sscanf(buf, "%lu-%lu", &lo, &hi) != EOF) {
                struct interval i = { .lo = lo, .hi = hi };
                node_insert(&root, i);
            } else {
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
    return 0;
}
