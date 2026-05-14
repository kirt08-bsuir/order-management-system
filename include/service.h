#ifndef SERVICE_H
#define SERVICE_H

#include "orders.h"
#include "product.h"
#include "order_items.h"

// read all 3 tables from files
void service_load_tables(
    ProductsTable *products_table,
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table
);

// save all 3 tables into files
void service_save_tables(
    const ProductsTable *products_table,
    const OrdersTable *orders_table,
    const OrderItemsTable *order_items_table
);

// add record into orders
// returns  0 on success
// returns -1 if order not found
// returns -2 if product not found
// returns  1 on other error
int service_add_item_to_order(
    ProductsTable *products_table,
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table,
    const unsigned int order_id,
    const unsigned int product_id,
    const unsigned int quantity
);

// returns 0 on success, -1 if not found, 1 on error
int service_delete_item_from_order(
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table,
    const unsigned int order_id,
    const unsigned int product_id
);

// returns 0 on success, -1 if order not found, 1 on error
int service_delete_order(
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table,
    const unsigned int order_id
);

void service_filter_products_by_quantity(
    ProductsTable *products_table,
    const unsigned int target
);

#endif