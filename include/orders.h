#ifndef ORDERS_H
#define ORDERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "base.h"


typedef struct {
    unsigned int order_id;
    char customer_name[ORDER_NAME_MAX];
    char order_date[ORDER_DATE_LEN];
    unsigned int total_sum;
    bool is_deleted;
} Order;

typedef struct {
    Order *original_table;

    SortedIndex *sorted_by_name;

    unsigned int capacity;
    unsigned int size;

    unsigned int last_id;
} OrdersTable;


OrdersTable *orders_table_create(void);

int orders_table_load(OrdersTable *orders_table);
int orders_table_save(const OrdersTable *orders_table);

// add new record into orders_table
// returns new_order_id or 0 if error occurs
unsigned int orders_table_add(OrdersTable *orders_table, const char *name);

// remove records where order_id == order_id
// returns 0 or -1 if record not found, 1 if error occurs
int orders_table_delete(OrdersTable *orders_table, const unsigned int order_id);

// edit record in orders_table
// returns 0 or -1 if not found, 1 if error occurs
int orders_table_edit(OrdersTable *orders_table, const unsigned int order_id, const char *customer_name);

// update total_sum field, will call from service layer
// returns 0 ot -1 if not dound, 1 if error occurs
// may be better to use here delta, because we can drop smth. from out order and then total_sum will decresse
// ???
int orders_table_update_total_sum(OrdersTable *orders_table, const unsigned int order_id, const int total_sum);

Order *orders_table_find_by_id(const OrdersTable *orders_table, const unsigned int order_id);

// print all records, where name == target_name
void orders_table_find_by_name(const OrdersTable *orders_table, const char *name);

void order_print(const Order* order);

void orders_table_print_all(const OrdersTable *orders_table);

void orders_table_print_sorted_by_customer_name(const OrdersTable *orders_table);

void orders_table_free(OrdersTable *orders_table);


#endif