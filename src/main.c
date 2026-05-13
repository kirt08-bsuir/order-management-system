#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"
#include "orders.h"
#include "product.h"


int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) {run_tests(); return 0;}

    OrdersTable *table = orders_table_create();
    if (!table) {
        return 1;
    }
    
    printf("--------------- START ---------------\n");

    char temp_name[50];
    for (unsigned int i = 0; i < ORDER_INITIAL_CAPACITY * ORDER_GROWTH_FACTOR + 4; i++) {
        snprintf(temp_name, sizeof(temp_name), "Test %d", i);
        orders_table_add(table, temp_name);
    }

    orders_table_save(table);
    orders_table_free(table);

    OrdersTable *table2 = orders_table_create();
    orders_table_load(table2);

    orders_table_print_all(table2);
    printf("-----------\n");
    

    return 0;
}