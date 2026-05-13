#include <string.h>

#include "tests.h"
#include "orders.h"
#include "config.h"

#define print_separator printf("------------------------------------------------------------------------------------------\n");


int test_integrity_orders_save_and_load(void) {
    OrdersTable *table = orders_table_create();

    char temp_name[50];
    for (unsigned int i = 0; i < ORDER_INITIAL_CAPACITY * 4; i++) {
        snprintf(temp_name, sizeof(temp_name), "Test %d", i);
        orders_table_add(table, temp_name);
    }

    if (DEBUG) orders_table_print_all(table);

    unsigned int old_capacity = table->capacity;
    unsigned int old_size = table->size;

    orders_table_save(table);

    orders_table_free(table);
    table = NULL;

    table = orders_table_create();
    orders_table_load(table);

    print_separator;

    if (DEBUG) orders_table_print_all(table);

    unsigned int new_capacity = table->capacity;
    unsigned int new_size = table->size;

    orders_table_free(table);
    table = NULL;

    if (new_capacity == old_capacity && new_size == old_size) return 1;
    return 0;
}