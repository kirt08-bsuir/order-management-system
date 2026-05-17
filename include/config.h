#ifndef CONFIG_H
#define CONDIG_H


#define DEBUG 0
#define DATA_FOLDER_PREFIX "data/"

// PRODUCT
#define PRODUCT_NAME_MAX 100
#define PRODUCT_INITIAL_CAPACITY 8
#define PRODUCT_GROWTH_FACTOR 1.1  // + 10% when extend
#define PRODUCT_FILE DATA_FOLDER_PREFIX "products.bin"

// ORDERS
#define ORDER_NAME_MAX 100
#define ORDER_DATE_LEN 11 // `dd.mm.yyyy\0`
#define ORDER_INITIAL_CAPACITY 8
#define ORDER_GROWTH_FACTOR 1.1  // + 10% when extend
#define ORDER_FILE DATA_FOLDER_PREFIX "orders.bin"

// ORDERS_ITEMS
#define ORDER_ITEMS_INITIAL_CAPACITY 8
#define ORDER_ITEMS_GROWTH_FACTOR 1.1
#define ORDER_ITEMS_FILE DATA_FOLDER_PREFIX "order_items.bin"


#endif