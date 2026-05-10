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