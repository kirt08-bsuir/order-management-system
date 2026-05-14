#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tests.h"
#include "config.h"
#include "orders.h"
#include "product.h"
#include "order_items.h" 


int test_unit_products_table_create(void) {
    ProductsTable *table = products_table_create();

    if (!table) return 0;
    if (table->size != 0) return 0;
    if (table->capacity <= 0) return 0;
    
    products_table_free(table);
    table = NULL;
    return 1;
}

int test_unit_orders_table_create(void) {
    OrdersTable *table = orders_table_create();

    if (!table) return 0;
    if (table->size != 0) return 0;
    if (table->capacity <= 0) return 0;

    orders_table_free(table);
    table = NULL;
    return 1;
}

int test_unit_order_items_table_create(void) {
    OrderItemsTable *table = order_items_table_create();

    if (!table) return 0;
    if (table->size != 0) return 0;
    if (table->capacity <= 0) return 0;
 
    order_items_table_free(table);
    table = NULL;
    return 1;
}

int test_unit_products_resize_capacity(void) {
    ProductsTable *table = products_table_create();
    if (!table) return 0;
    
    unsigned int old_capacity = table->capacity;
    
    for (unsigned int i = 0; i < old_capacity + 2; i++) products_table_add(table, "Test Item", 100, 10);

    if (table->capacity <= old_capacity) return 0;
    
    products_table_free(table);
    table = NULL;
    
    return 1;
}

int test_unit_orders_table_resize_capacity(void) {
    OrdersTable *table = orders_table_create();
    if (!table) return 0;
    
    unsigned int old_capacity = table->capacity;
    
    for (unsigned int i = 0; i < old_capacity + 2; i++) orders_table_add(table, "Test Item");
    if (table->capacity <= old_capacity) return 0;
    
    orders_table_free(table);
    table = NULL;
    
    return 1;
}

int test_unit_order_items_table_resize_capacity(void) {
    OrderItemsTable *table = order_items_table_create();
    if (!table) return 0;
 
    unsigned int old_capacity = table->capacity;
 
    for (unsigned int i = 0; i < old_capacity + 2; i++) {
        order_items_table_add(table, 1, i + 1, 10);
    }
 
    if (table->capacity <= old_capacity) return 0;
 
    order_items_table_free(table);
    table = NULL;
 
    return 1;
}

int test_unit_order_items_add_new_item(void) {
    OrderItemsTable *table = order_items_table_create();
 
    int res = order_items_table_add(table, 1, 100, 5);
 
    if (res != 0) {
        order_items_table_free(table);
        return 0;
    }
 
    if (table->size != 1) {
        order_items_table_free(table);
        return 0;
    }
 
    order_items_table_free(table);
    table = NULL;
 
    return 1;
}
 
int test_unit_order_items_add_duplicate_pair_increases_quantity(void) {
    OrderItemsTable *table = order_items_table_create();
 
    order_items_table_add(table, 1, 100, 5);
    unsigned int size_after_first = table->size;
 
    order_items_table_add(table, 1, 100, 3);
    unsigned int size_after_second = table->size;
 
    if (size_after_second != size_after_first) {
        order_items_table_free(table);
        return 0;
    }
 
    OrderItem *item = order_items_table_find_order_item(table, 1, 100);
    if (!item || item->quantity != 8) {
        order_items_table_free(table);
        return 0;
    }
 
    order_items_table_free(table);
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

int test_unit_orders_delete_non_existing_record(void) {
    OrdersTable *table = orders_table_create();

    int res = orders_table_delete(table, 10);

    orders_table_free(table);
    table = NULL;

    if (res == -1) return 1; else return 0;
}

int test_unit_order_items_delete_non_existing_record(void) {
    OrderItemsTable *table = order_items_table_create();
 
    int res = order_items_table_delete(table, 1, 100);
 
    order_items_table_free(table);
    table = NULL;
 
    if (res == 1) return 1;
    return 0;
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

int test_unit_orders_delete_existing_record(void) {
    OrdersTable *table = orders_table_create();

    orders_table_add(table, "TEST");
    int res = orders_table_delete(table, 1);

    orders_table_free(table);
    table = NULL;

    if (res == 0) return 1;
    return 0;
}

int test_unit_order_items_delete_existing_record(void) {
    OrderItemsTable *table = order_items_table_create();
 
    order_items_table_add(table, 1, 100, 5);
    int res = order_items_table_delete(table, 1, 100);
 
    order_items_table_free(table);
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

int test_unit_orders_delete_existing_record_after_expanding(void) {
    OrdersTable *table = orders_table_create();

    unsigned int old_capacity = table->capacity;
    for (unsigned int i = 0; i < old_capacity + 2; i++) orders_table_add(table, "TEST");
    
    int res = orders_table_delete(table, 1);

    orders_table_free(table);
    table = NULL;
    
    if (res == 0) return 1;
    return 0;
}

int test_unit_order_items_delete_existing_record_after_expanding(void) {
    OrderItemsTable *table = order_items_table_create();
 
    unsigned int old_capacity = table->capacity;
    for (unsigned int i = 0; i < old_capacity + 2; i++) order_items_table_add(table, 1, i + 1, 10);
 
    int res = order_items_table_delete(table, 1, 1);
 
    order_items_table_free(table);
    table = NULL;
 
    if (res == 0) return 1;
    return 0;
}

int test_unit_products_editing_invalid(void) {
    ProductsTable *table = products_table_create();

    int res = products_table_edit_record(table, 2, "", 0, false, 0);

    products_table_free(table);
    table = NULL;

    if (res == -1) return 1; else return 0;
}

int test_unit_orders_editing_invalid(void) {
    OrdersTable *table = orders_table_create();

    orders_table_add(table, "TEST");
    int res = orders_table_edit(table, 1, "");

    orders_table_free(table);
    table = NULL;

    if (res == 1) return 1;
    return 0;
}

int test_unit_order_items_edit_quantity_invalid(void) {
    OrderItemsTable *table = order_items_table_create();
 
    int res = order_items_table_edit_quantity(table, 1, 100, 20);
 
    order_items_table_free(table);
    table = NULL;
 
    if (res == -1) return 1;
    return 0;
}

int test_unit_products_editing_valid(void) {
    ProductsTable *table = products_table_create();

    products_table_add(table, "Test", 1, 1);
    int res = products_table_edit_record(table, 1, "Unit-Test", 0, false, 0);

    products_table_free(table);
    table = NULL;

    if (res == 0) return 1; else return 0;
}

int test_unit_orders_editing_valid(void) {
    OrdersTable *table = orders_table_create();

    orders_table_add(table, "TEST");
    int res = orders_table_edit(table, 1, "SUPER-TEST");

    orders_table_free(table);
    table = NULL;

    if (res == 0) return 1;
    return 0;
}

int test_unit_order_items_edit_quantity_valid(void) {
    OrderItemsTable *table = order_items_table_create();
 
    order_items_table_add(table, 1, 100, 5);
    int res = order_items_table_edit_quantity(table, 1, 100, 20);
 
    if (res != 0) {
        order_items_table_free(table);
        return 0;
    }
 
    OrderItem *item = order_items_table_find_order_item(table, 1, 100);
    if (!item || item->quantity != 20) {
        order_items_table_free(table);
        return 0;
    }
 
    order_items_table_free(table);
    table = NULL;
 
    return 1;
}

int test_unit_products_search_by_id_non_existing_record(void) {
    ProductsTable *table = products_table_create();

    Product *p = products_table_search_by_id(table, 1);

    products_table_free(table);
    table = NULL;

    if (p == NULL) return 1; else return 0;
}

int test_unit_orders_search_by_id_non_existing_record(void) {
    OrdersTable *table = orders_table_create();

    Order *o = orders_table_find_by_id(table, 1);

    orders_table_free(table);
    table = NULL;

    if (o == NULL) return 1;
    return 0;
}

int test_unit_order_items_find_by_order_id_non_existing_order(void) {
    OrderItemsTable *table = order_items_table_create();
 
    OrderItem **items = order_items_table_find_by_order(table, 999);
 
    order_items_table_free(table);
    table = NULL;
 
    if (items == NULL) return 1;
    return 0;
}

int test_unit_products_search_by_id_existing_record(void) {
    ProductsTable *table = products_table_create();

    products_table_add(table, "Test", 1, 1);
    Product *p = products_table_search_by_id(table, 1);

    products_table_free(table);
    table = NULL;

    if (p != NULL) return 1; else return 0;
}

int test_unit_orders_search_by_id_existing_record(void) {
    OrdersTable *table = orders_table_create();

    orders_table_add(table, "TEST");
    Order *o = orders_table_find_by_id(table, 1);

    orders_table_free(table);
    table = NULL;

    if (o != NULL) return 1;
    return 0;
}

int test_unit_order_items_find_by_order_id_existing_order(void) {
    OrderItemsTable *table = order_items_table_create();
    if (!table) return 0;
 
    order_items_table_add(table, 1, 100, 5);
    order_items_table_add(table, 1, 101, 3);
 
    OrderItem **items = order_items_table_find_by_order(table, 1);
 
    if (!items) {
        order_items_table_free(table);
        return 0;
    }
 
    /* Check that we got 2 items and sentinel NULL */
    if (items[0] == NULL || items[1] == NULL || items[2] != NULL) {
        free(items);
        order_items_table_free(table);
        return 0;
    }
 
    free(items);
    order_items_table_free(table);
    table = NULL;
 
    return 1;
}

int test_unit_order_items_get_total_cost_non_existing(void) {
    OrderItemsTable *table = order_items_table_create();
 
    int cost = order_items_table_get_total_cost(table, 1, 100);
 
    order_items_table_free(table);
    table = NULL;
 
    if (cost == -1) return 1; 
    return 0;
}
 
int test_unit_order_items_get_total_cost_existing(void) {
    OrderItemsTable *table = order_items_table_create();
 
    order_items_table_add(table, 1, 100, 5);
    order_items_table_set_total_cost(table, 1, 100, 500);
 
    int cost = order_items_table_get_total_cost(table, 1, 100);
 
    order_items_table_free(table);
    table = NULL;
 
    if (cost == 500) return 1;
    return 0;
}

int test_unit_order_items_set_total_cost_non_existing(void) {
    OrderItemsTable *table = order_items_table_create();
 
    int res = order_items_table_set_total_cost(table, 1, 100, 500);
 
    order_items_table_free(table);
    table = NULL;
 
    if (res == -1) return 1; 
    return 0;
}