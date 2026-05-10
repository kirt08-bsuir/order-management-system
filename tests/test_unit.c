#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tests.h"
#include "config.h"
#include "product.h" 


int test_unit_products_table_create(void) {
    ProductsTable *table = products_table_create();
    if (!table) return 0;
    if (table->size != 0) return 0;
    if (table->capacity <= 0) return 0;
    
    products_table_free(table);
    table = NULL;
    return 1;
}

int test_unit_products_resize_capacity(void) {
    ProductsTable *table = products_table_create();
    if (!table) return 0;
    
    unsigned int old_capacity = table->capacity;
    
    // Искусственно заполняем size, чтобы спровоцировать realloc при добавлении
    table->size = old_capacity; 
    for (unsigned int i = 0; i < PRODUCT_INITIAL_CAPACITY; i++) products_table_add(table, "Test Item", 100, 10);
    
    if (table->capacity <= old_capacity) return 0;
    
    products_table_free(table);
    table = NULL;
    
    return 1;
}

int test_unit_products_delete_non_existing_record(void) {
    ProductsTable *table = products_table_create();

    int res = products_table_delete_by_id(table, 10);

    products_table_free(table);
    table = NULL;

    if (res == -1) return 1; else return 0;
}

int test_unit_products_delete_existing_record(void) {
    ProductsTable *table = products_table_create();

    products_table_add(table, "Test", 1, 1);
    int res = products_table_delete_by_id(table, 1);
    
    products_table_free(table);
    table = NULL;

    if (res == 0) return 1;
    return 0;
}

int test_unit_products_delete_existing_record_after_expanding(void) {
    ProductsTable *table = products_table_create();

    for (unsigned int i = 0; i < PRODUCT_INITIAL_CAPACITY * PRODUCT_GROWTH_FACTOR + 2; i++) {
        products_table_add(table, "Test", i + 1, i + 1);
    }

    int res = products_table_delete_by_id(table, 1);
    
    products_table_free(table);
    table = NULL;

    if (res == 0) return 1; else return 0;
}

int test_unit_products_editing_invalid(void) {
    ProductsTable *table = products_table_create();

    int res = products_table_edit_record(table, 2, "", 0, false, 0);

    products_table_free(table);
    table = NULL;

    if (res == -1) return 1; else return 0;
}

int test_unit_products_editing_valid(void) {
    ProductsTable *table = products_table_create();

    products_table_add(table, "Test", 1, 1);
    int res = products_table_edit_record(table, 1, "Unit-Test", 0, false, 0);

    products_table_free(table);
    table = NULL;

    if (res == 0) return 1; else return 0;
}