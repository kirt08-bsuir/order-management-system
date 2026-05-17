#ifndef SERVICE_H
#define SERVICE_H

#include "orders.h"
#include "product.h"
#include "order_items.h"
 

// read all 3 tables from files
int service_load_tables(
    ProductsTable *products_table,
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table
);

// save all 3 tables into files
int service_save_tables(
    const ProductsTable *products_table,
    const OrdersTable *orders_table,
    const OrderItemsTable *order_items_table
);

// add record into orders
// returns  0 on success
// returns -1 if order not found
// returns -2 if product not found
// returns -3 if proudtc->quantity < need quantity
// returns  1 on other error
int service_add_item_to_order(
    ProductsTable *products_table,
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table,
    const unsigned int order_id,
    const unsigned int product_id,
    const unsigned int quantity
);


// returns 0 on success
// returns 1 if something went wrong
// returns 2 if some order include record and price can't be changed
int service_edit_product_record(
    ProductsTable *products_table,
    const OrderItemsTable *order_items_table,
    const unsigned int product_id,
    const char *name,
    const unsigned int unit_price,
    const bool change_quantity,
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
    ProductsTable *products_table,
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table,
    const unsigned int order_id
);

void service_filter_products_by_quantity(
    ProductsTable *products_table,
    const unsigned int target
);


int service_print_invoice(
    const ProductsTable *products_table,
    const OrdersTable *orders_table,
    const OrderItemsTable *order_items_table,
    const unsigned int order_id
);
 
void service_print_all_invoices(
    const ProductsTable *products_table,
    const OrdersTable *orders_table,
    const OrderItemsTable *order_items_table
);


#endif