#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "base.h"
#include "utils.h"
#include "config.h"
#include "orders.h"


static int _orders_resize(OrdersTable *orders_table) {
    if (!orders_table) return -1;
 
    unsigned int new_capacity = (unsigned int)(orders_table->capacity * ORDER_GROWTH_FACTOR);
    if (new_capacity == orders_table->capacity) new_capacity++;
 
    Order *new_table = (Order *)realloc(orders_table->original_table, new_capacity * sizeof(Order));
    if (!new_table) {
        if (DEBUG) printf("Something went wrong during allocation new memory for OrdersTable->original_table with number of records %d.\n", new_capacity);
        return -1;
    }
    orders_table->original_table = new_table;
 
    SortedIndex *new_name_idx = (SortedIndex *)realloc(orders_table->sorted_by_name, new_capacity * sizeof(SortedIndex));
    if (!new_name_idx) {
        if (DEBUG) printf("Something went wrong during allocation new memory for OrdersTable->sorted_by_name with number of records %d.\n", new_capacity);
        return -1;
    }
    orders_table->sorted_by_name = new_name_idx;
 
    orders_table->capacity = new_capacity;
    return 0;
}

static int _names_comparator(const char *s1, const char *s2) {
    if (!s1 || !s2) return -1;
 
    while (*s1 && *s2) {
        int c1 = tolower((unsigned char)*s1);
        int c2 = tolower((unsigned char)*s2);
        if (c1 != c2) return c1 - c2;
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

static int _orders_binary_search_by_id(const OrdersTable *orders_table, const unsigned int order_id) {
    if (!orders_table || orders_table->size == 0) return -1;
 
    int left = 0, right = (int)orders_table->size - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        unsigned int mid_id = orders_table->original_table[mid].order_id;
 
        if (mid_id == order_id) return mid;
 
        if (mid_id < order_id) left = mid + 1; else right = mid - 1;
    }
 
    return -1;
}

static void _orders_rebuild_index_by_name(OrdersTable *orders_table) {
    if (!orders_table || orders_table->size == 0) return;
 
    for (unsigned int i = 0; i < orders_table->size; i++) orders_table->sorted_by_name[i].idx = i;
 
    for (unsigned int i = 0; i < orders_table->size - 1; i++) {
        for (unsigned int j = i + 1; j < orders_table->size; j++) {
            unsigned int idx1 = orders_table->sorted_by_name[i].idx;
            unsigned int idx2 = orders_table->sorted_by_name[j].idx;
 
            if (_names_comparator(
                orders_table->original_table[idx1].customer_name,
                orders_table->original_table[idx2].customer_name
            ) > 0) {
                SortedIndex tmp = orders_table->sorted_by_name[i];
                orders_table->sorted_by_name[i] = orders_table->sorted_by_name[j];
                orders_table->sorted_by_name[j] = tmp;
            }
        }
    }
}

OrdersTable *orders_table_create(void) {
    OrdersTable *orders_table = (OrdersTable *)malloc(sizeof(OrdersTable));
    if (!orders_table) {
        if (DEBUG) printf("Something went wrong during allocation memory for OrdersTable.\n");
        return NULL;
    }
 
    orders_table->original_table = (Order *)malloc(ORDER_INITIAL_CAPACITY * sizeof(Order));
    if (!orders_table->original_table) {
        if (DEBUG) printf("Something went wrong during allocation memory for OrdersTable->original_table with number of records %d.\n", ORDER_INITIAL_CAPACITY);
        free(orders_table);
        return NULL;
    }
 
    orders_table->sorted_by_name = (SortedIndex *)malloc(ORDER_INITIAL_CAPACITY * sizeof(SortedIndex));
    if (!orders_table->sorted_by_name) {
        if (DEBUG) printf("Something went wrong during allocation memory for OrdersTable->sorted_by_name with number of records %d.\n", ORDER_INITIAL_CAPACITY);
        free(orders_table->original_table);
        free(orders_table);
        return NULL;
    }
 
    orders_table->capacity = ORDER_INITIAL_CAPACITY;
    orders_table->size = 0;
    orders_table->last_id = 0;
 
    return orders_table;
}

unsigned int orders_table_add(OrdersTable *orders_table, const char *name) {
    if (!orders_table) return 0;
 
    if (!validate_non_empty_string(name)) {
        printf("Validation Error: invalid value for field `customer_name`.\n");
        return 0;
    }
 
    if (orders_table->size >= orders_table->capacity) {
        if (_orders_resize(orders_table) != 0) return 0;
    }
 
    unsigned int new_id = ++(orders_table->last_id);
 
    Order *o = &orders_table->original_table[orders_table->size];
 
    o->order_id = new_id;
    o->total_sum = 0;
    o->is_deleted = false;
 
    strncpy(o->customer_name, name, ORDER_NAME_MAX - 1);
    o->customer_name[ORDER_NAME_MAX - 1] = '\0';
 
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    snprintf(o->order_date, 11, "%02d.%02d.%04d", 
            tm_info->tm_mday, 
            tm_info->tm_mon + 1, 
            tm_info->tm_year + 1900);
 
    orders_table->size++;
 
    _orders_rebuild_index_by_name(orders_table);
 
    return new_id;
}

int orders_table_delete(OrdersTable *orders_table, const unsigned int order_id) {
    if (!orders_table) return 1;
 
    int idx = _orders_binary_search_by_id(orders_table, order_id);
    if (idx == -1) {
        if (DEBUG) printf("Record wasn't found.\n");
        return -1;
    }
 
    orders_table->original_table[idx].is_deleted = true;
    return 0;
}

int orders_table_edit(OrdersTable *orders_table, const unsigned int order_id, const char *customer_name) {
    if (!orders_table) return 1;
 
    int idx = _orders_binary_search_by_id(orders_table, order_id);
    if (idx == -1) {
        if (DEBUG) printf("Record wasn't found.\n");
        return -1;
    }
 
    if (!validate_non_empty_string(customer_name)) {
        printf("Validation Error: invalid value for field `customer_name`.\n");
        return 1;
    }
 
    Order *o = &orders_table->original_table[idx];
 
    strncpy(o->customer_name, customer_name, ORDER_NAME_MAX - 1);
    o->customer_name[ORDER_NAME_MAX - 1] = '\0';
 
    _orders_rebuild_index_by_name(orders_table);
 
    return 0;
}

int orders_table_update_total_sum(OrdersTable *orders_table, const unsigned int order_id, const int total_sum) {
    if (!orders_table) return 1;
 
    int idx = _orders_binary_search_by_id(orders_table, order_id);
    if (idx == -1) {
        if (DEBUG) printf("Record wasn't found.\n");
        return -1;
    }
 
    // total_sum - delta:
    // it can be positive if we add something or negative if we drop something

    Order *o = &orders_table->original_table[idx];
    int new_sum = (int)o->total_sum + total_sum;
    o->total_sum = new_sum > 0 ? (unsigned int)new_sum : 0;
 
    return 0;
}

Order *orders_table_find_by_id(const OrdersTable *orders_table, const unsigned int order_id) {
    if (!orders_table) return NULL;
 
    int idx = _orders_binary_search_by_id(orders_table, order_id);
    if (idx == -1) {
        if (DEBUG) printf("Record wasn't found.\n");
        return NULL;
    }
 
    Order *o = &orders_table->original_table[idx];
    if (o->is_deleted) return NULL;
 
    return o;
}

void order_print(const Order *o) {
    if (!o) return;
 
    printf("  ID: %-5u | ", o->order_id);
    printf("Customer: %-30s | ", o->customer_name);
    printf("Date: %-12s | ", o->order_date[0] ? o->order_date : "—");
    printf("Total: %u", o->total_sum);
 
    if (o->is_deleted) printf(" [DELETED]");
    printf("\n");
}
 
void orders_table_print_all(const OrdersTable *orders_table) {
    if (!orders_table) {
        if (DEBUG) printf("Something went wrong during showing all records from orders table.\n");
        return;
    }
 
    int count = 0;
    for (unsigned int i = 0; i < orders_table->size; i++) {
        if (!orders_table->original_table[i].is_deleted) {
            order_print(&orders_table->original_table[i]);
            count++;
        }
    }
 
    if (count == 0) printf("Orders table is empty.\n");
}
 
void orders_table_print_sorted_by_customer_name(const OrdersTable *orders_table) {
    if (!orders_table) return;
 
    int count = 0;
    for (unsigned int i = 0; i < orders_table->size; i++) {
        unsigned int idx = orders_table->sorted_by_name[i].idx;
        if (!orders_table->original_table[idx].is_deleted) {
            order_print(&orders_table->original_table[idx]);
            count++;
        }
    }
 
    if (count == 0) printf("Orders table is empty.\n");
}

void orders_table_free(OrdersTable *orders_table) {
    if (!orders_table) return;
 
    if (orders_table->sorted_by_name) {
        free(orders_table->sorted_by_name);
        orders_table->sorted_by_name = NULL;
    }
 
    if (orders_table->original_table) {
        free(orders_table->original_table);
        orders_table->original_table = NULL;
    }
 
    free(orders_table);
}