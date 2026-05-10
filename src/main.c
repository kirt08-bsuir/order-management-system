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
    
    printf("--------------- START ---------------\n");

    ProductsTable *table = products_table_create();

    char temp_name[50];
    for (unsigned int i = 0; i < PRODUCT_INITIAL_CAPACITY * PRODUCT_GROWTH_FACTOR + 4; i++) {
        snprintf(temp_name, sizeof(temp_name), "Test %d", i);
        products_table_add(table, temp_name, i + 1, 30 - i);
    }
    products_table_print_all(table);
    printf("-----------\n");
    products_table_delete_by_id(table, 10);
    products_table_filter_by_quantity(table, 25);

    return 0;
}