#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "product.h"
#include "tests.h"


int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {run_tests(); return 0;}
    ProductsTable *products_table = products_table_create();
    if (!products_table) {
        return 1;
    }

    return 0;
}