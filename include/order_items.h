#ifndef ORDER_ITEMS_H
#define ORDER_ITEMS_H

#include <stdbool.h>

#include "config.h"


typedef struct {
    unsigned int order_id;
    unsigned int product_id;
    unsigned int quantity;
    unsigned int total_cost;
    bool is_deleted;
} OrderItem;

typedef struct {
    OrderItem *original_table;
    unsigned int capacity;
    unsigned int size;
} OrderItemsTable;


OrderItemsTable *order_items_table_create(void);

int order_items_table_load(OrderItemsTable *table);
int order_items_table_save(const OrderItemsTable *table);

// return 0, 1 if error occurs
int order_items_table_add(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id,
    const unsigned int quantity
);

// return 0, 1 if error occurs
int order_items_table_delete(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id
);

// return 0, -1 if (order_id, product_id) wasn't found, 1 if error occurs
int order_items_table_edit_quantity(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id,
    const unsigned int new_quantity
);

int order_items_table_get_total_cost(const OrderItemsTable *table, const unsigned int order_id, const unsigned int product_id);
int order_items_table_set_total_cost(OrderItemsTable *table, const unsigned int order_id, const unsigned int product_id, const unsigned int new_total_cost);

OrderItem *order_items_table_find_order_item(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id
);

OrderItem *order_items_table_find_order_with_product_id(
    const OrderItemsTable *table,
    const unsigned int product_id
);

OrderItem **order_items_table_find_by_order(
    OrderItemsTable *table,
    const unsigned int order_id
);

void order_item_print(const OrderItem *item);

void order_items_table_print_all(const OrderItemsTable *table);
void order_items_table_print_all_by_order(const OrderItemsTable *table, const unsigned int order_id);

void order_items_table_free(OrderItemsTable *table);

#endif