#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

#define BUF_SIZE 512
#define VEC_SIZE 256

enum light {
    ON,
    OFF
};

static enum light on = ON;
static enum light off = OFF;

void print_indicator(const struct vec_t* indicator) {
    printf("[");

    for (int i = 0; i < vec_size(indicator); ++i) {
        enum light* light = vec_get(indicator, i);

        printf("%c", *light == OFF ? '.' : '#');

        if (i == vec_size(indicator) - 1) printf("]");
    }
}

void print_button(const struct vec_t* button) {
    printf("(");

    for (int i = 0; i < vec_size(button); ++i) {
        int* b = vec_get(button, i);
        printf("%d", *b);
        if (i == vec_size(button) - 1) {
            printf(")");
        } else {
            printf(", ");
        }
    }
}

void print_buttons(const struct vec_t* buttons) {
    for (int i = 0; i < vec_size(buttons); ++i) {
        struct vec_t* button = vec_get(buttons, i);
        print_button(button);
        if (i < vec_size(buttons) - 1) {
            printf(", ");
        }
    }
}

void print_joltages(const struct vec_t* joltages) {
    for (int i = 0; i < vec_size(joltages); ++i) {
        int* n = vec_get(joltages, i);
        printf("%d", *n);

        if (i < vec_size(joltages) - 1) {
            printf(", ");
        }
    }
}

int main()
{
    char buf[BUF_SIZE];

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0';
        buf_len--;

        // parse input
        char* outer_saveptr = NULL;
        char* inner_saveptr = NULL;
        char* next = strtok_r(buf, " ", &outer_saveptr);
        size_t next_len = strlen(next);

        struct vec_t* indicator = vec_new(VEC_SIZE, sizeof(enum light));

        if (next[0] == '[') {
            for (int i = 1; i < next_len - 1; ++i) {
                if (next[i] == '.') {
                    vec_insert(indicator, &off);
                } else if (next[i] == '#') {
                    vec_insert(indicator, &on);
                } else {
                    printf("Unrecognized input: %s at %d\n", next, i);
                    exit(-1);
                }
            }
        }

        struct vec_t* buttons = vec_new(VEC_SIZE, sizeof (struct vec_t));
        struct vec_t* joltages = vec_new(VEC_SIZE, sizeof (int));

        while ((next = strtok_r(NULL, " ", &outer_saveptr))) {
            next_len = strlen(next);

            if (next[0] == '(') {
                next = next+1;
                next[next_len-1] = '\0';

                struct vec_t* button = vec_new(VEC_SIZE, sizeof (int));
                char* ntok = strtok_r(next, ",", &inner_saveptr);
                int* n = malloc(sizeof (int));
                *n = atoi(ntok);
                vec_insert(button, n);

                while ((ntok = strtok_r(NULL, ",", &inner_saveptr))) {
                    n = malloc(sizeof (int));
                    *n = atoi(ntok);
                    vec_insert(button, n);
                }

                vec_insert(buttons, button);
            }

            if (next[0] == '{') {
                next = next+1;
                next[next_len-1] = '\0';

                char* ntok = strtok_r(next, ",", &inner_saveptr);
                int* n = malloc(sizeof (int));
                *n = atoi(ntok);
                vec_insert(joltages, n);

                while ((ntok = strtok_r(NULL, ",", &inner_saveptr))) {
                    n = malloc(sizeof (int));
                    *n = atoi(ntok);
                    vec_insert(joltages, n);
                }
            }
        }

        print_indicator(indicator);
        printf(" - ");
        print_buttons(buttons);
        printf(" - ");
        print_joltages(joltages);
        printf("\n");
    }

    return 0;
}
