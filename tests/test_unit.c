#include <stdio.h>
#include <stdlib.h>

#include "tests.h"
#include "config.h"
#include "product.h" 


int test_unit_table_create(void) {
    ProductsTable *table = products_table_create();
    if (!table) return 0;
    if (table->size != 0) return 0;
    if (table->capacity <= 0) return 0;
    
    products_table_free(table);
    table = NULL;
    return 1;
}

int test_unit_resize_capacity(void) {
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