#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"


bool validate_non_empty_string(const char *str) {
    if (!str) return false;
    return strlen(str) > 0;
}

bool validate_price(const unsigned int price) {
    return price > 0;
}

char input_get_char(void) {
    int ch = getchar();
    while (getchar() != '\n');
    return (char)ch;
}

void input_get_string(char *buf, int max_len) {
    if (!buf || max_len <= 0) return;
 
    if (fgets(buf, max_len, stdin) != NULL) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }
    } else {
        buf[0] = '\0';
    }
 
    if (strlen(buf) == (size_t)(max_len - 1) && buf[max_len - 2] != '\n') {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
    }
}

unsigned int input_get_uint(void) {
    int value, result;
 
    while (1) {
        result = scanf("%d", &value);
 
        if (result == 1 && value >= 0) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            return (unsigned int)value;
        } else {
            printf("Invalid input. Please enter a valid number.");
 
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
 
            printf("  Try again: ");
        }
    }
}