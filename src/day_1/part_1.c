#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define VAULT_MAX 100

int main()
{
    char buf[20];

    int current_num = 50;
    int zeroes_count = 0;

    while (fgets(buf, sizeof (buf), stdin) != NULL) {
        char turn = buf[0];
        int amt = strtol(buf+1, NULL, 10);

        assert(current_num >= 0 && current_num <= 99);

        if (turn == 'L') {
            int next = (current_num - amt) % VAULT_MAX;

            if (next < 0) {
                current_num = VAULT_MAX + next;
            } else {
                current_num = next;
            }
        }

        if (turn == 'R') {
            current_num = (current_num + amt) % VAULT_MAX;
        }

        if (current_num == 0) {
            zeroes_count++;
        }
    }

    printf("Day 1: %d\n", zeroes_count);

    return 0;
}
