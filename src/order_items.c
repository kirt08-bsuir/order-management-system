#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "order_items.h"


static int _order_items_resize(OrderItemsTable *table) {
    if (!table) return -1;

    unsigned int new_capacity = (unsigned int)(table->capacity * ORDER_ITEMS_GROWTH_FACTOR);
    if (new_capacity == table->capacity) new_capacity++;

    OrderItem *new_table = (OrderItem *)realloc(table->original_table, new_capacity * sizeof(OrderItem));
    if (!new_table) {
        if (DEBUG) printf("Something went wrong during allocation new memory for OrderItemsTable->original_table with number of records %d.\n", new_capacity);
        return -1;
    }

    table->original_table = new_table;
    table->capacity = new_capacity;
    return 0;
}

/* Linear search by (order_id, product_id) pair.
 * Returns index in original_table or -1 if not found.
 * Searches including is_deleted records — needed for uniqueness check. */
static int _order_items_find_idx(
    const OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id
) {
    if (!table) return -1;

    for (unsigned int i = 0; i < table->size; i++) {
        OrderItem *item = &table->original_table[i];
        if (item->order_id == order_id && item->product_id == product_id && !item->is_deleted) {
            return (int)i;
        }
    }

    return -1;
}


OrderItemsTable *order_items_table_create(void) {
    OrderItemsTable *table = (OrderItemsTable *)malloc(sizeof(OrderItemsTable));
    if (!table) {
        if (DEBUG) printf("Something went wrong during allocation memory for OrderItemsTable.\n");
        return NULL;
    }

    table->original_table = (OrderItem *)malloc(ORDER_ITEMS_INITIAL_CAPACITY * sizeof(OrderItem));
    if (!table->original_table) {
        if (DEBUG) printf("Something went wrong during allocation memory for OrderItemsTable->original_table with number of records %d.\n", ORDER_ITEMS_INITIAL_CAPACITY);
        free(table);
        return NULL;
    }

    table->capacity = ORDER_ITEMS_INITIAL_CAPACITY;
    table->size = 0;

    return table;
}

int order_items_table_load(OrderItemsTable *table) {
    if (!table) return 1;

    FILE *f = fopen(ORDER_ITEMS_FILE, "rb");
    if (!f) {
        if (DEBUG) printf("Cannot open file %s for reading.\n", ORDER_ITEMS_FILE);
        return 1;
    }

    unsigned int size = 0;

    if (fread(&size, sizeof(unsigned int), 1, f) != 1) {
        if (DEBUG) printf("Cannot read size from %s.\n", ORDER_ITEMS_FILE);
        fclose(f);
        return 1;
    }

    while (table->capacity < size) {
        if (_order_items_resize(table) != 0) {
            fclose(f);
            return 1;
        }
    }

    for (unsigned int i = 0; i < size; i++) {
        if (fread(&table->original_table[i], sizeof(OrderItem), 1, f) != 1) {
            if (DEBUG) printf("Error reading order_item record from %s.\n", ORDER_ITEMS_FILE);
            fclose(f);
            return 1;
        }
    }

    table->size = size;

    fclose(f);
    return 0;
}

int order_items_table_save(const OrderItemsTable *table) {
    if (!table) return 1;

    FILE *f = fopen(ORDER_ITEMS_FILE, "wb");
    if (!f) {
        if (DEBUG) printf("Cannot open file %s for writing.\n", ORDER_ITEMS_FILE);
        return 1;
    }

    unsigned int saved_count = 0;
    for (unsigned int i = 0; i < table->size; i++) {
        if (!table->original_table[i].is_deleted) saved_count++;
    }

    if (fwrite(&saved_count, sizeof(unsigned int), 1, f) != 1) {
        if (DEBUG) printf("Error writing size to %s.\n", ORDER_ITEMS_FILE);
        fclose(f);
        return -1;
    }

    for (unsigned int i = 0; i < table->size; i++) {
        if (!table->original_table[i].is_deleted) {
            if (fwrite(&table->original_table[i], sizeof(OrderItem), 1, f) != 1) {
                if (DEBUG) printf("Error writing order_item record to %s.\n", ORDER_ITEMS_FILE);
                fclose(f);
                return -1;
            }
        }
    }

    fclose(f);
    return 0;
}

int order_items_table_add(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id,
    const unsigned int quantity
) {
    if (!table) return 1;

    int existing_idx = _order_items_find_idx(table, order_id, product_id);
    if (existing_idx != -1) {
        table->original_table[existing_idx].quantity += quantity;
        // total_cost recounting in service layer
        return 0;
    }

    if (table->size >= table->capacity) {
        if (_order_items_resize(table) != 0) return 1;
    }

    OrderItem *item = &table->original_table[table->size];

    item->order_id   = order_id;
    item->product_id = product_id;
    item->quantity   = quantity;
    item->total_cost = 0;
    item->is_deleted = false;

    table->size++;
    return 0;
}

int order_items_table_delete(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id
) {
    if (!table) return 1;

    int idx = _order_items_find_idx(table, order_id, product_id);
    if (idx == -1) {
        if (DEBUG) printf("Record (order_id=%u, product_id=%u) wasn't found.\n", order_id, product_id);
        return 1;
    }

    table->original_table[idx].is_deleted = true;
    return 0;
}

int order_items_table_edit_quantity(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id,
    const unsigned int new_quantity
) {
    if (!table) return 1;

    int idx = _order_items_find_idx(table, order_id, product_id);
    if (idx == -1) {
        if (DEBUG) printf("Record (order_id=%u, product_id=%u) wasn't found.\n", order_id, product_id);
        return -1;
    }

    table->original_table[idx].quantity = new_quantity;
    // total_cost recounting in service layer
    return 0;
}

int order_items_table_get_total_cost(
    const OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id
) {
    if (!table) return -1;
 
    int idx = _order_items_find_idx(table, order_id, product_id);
    if (idx == -1) {
        if (DEBUG) printf("Record (order_id=%u, product_id=%u) wasn't found.\n", order_id, product_id);
        return -1;
    }
 
    return (int)table->original_table[idx].total_cost;
}
 
int order_items_table_set_total_cost(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id,
    const unsigned int new_total_cost
) {
    if (!table) return 1;
 
    int idx = _order_items_find_idx(table, order_id, product_id);
    if (idx == -1) {
        if (DEBUG) printf("Record (order_id=%u, product_id=%u) wasn't found.\n", order_id, product_id);
        return -1;
    }
 
    table->original_table[idx].total_cost = new_total_cost;
    return 0;
}

OrderItem *order_items_table_find_order_item(
    OrderItemsTable *table,
    const unsigned int order_id,
    const unsigned int product_id
) {
    if (!table) return NULL;

    int idx = _order_items_find_idx(table, order_id, product_id);
    if (idx == -1) {
        if (DEBUG) printf("Record (order_id=%u, product_id=%u) wasn't found.\n", order_id, product_id);
        return NULL;
    }

    return &table->original_table[idx];
}

OrderItem **order_items_table_find_by_order(
    OrderItemsTable *table,
    const unsigned int order_id
) {
    if (!table) return NULL;

    unsigned int count = 0;
    for (unsigned int i = 0; i < table->size; i++) {
        OrderItem *item = &table->original_table[i];
        if (item->order_id == order_id && !item->is_deleted) count++;
    }

    if (count == 0) return NULL;

    OrderItem **results = (OrderItem **)malloc((count + 1) * sizeof(OrderItem *));
    if (!results) {
        if (DEBUG) printf("Something went wrong during allocation memory for order_items search results.\n");
        return NULL;
    }

    unsigned int j = 0;
    for (unsigned int i = 0; i < table->size; i++) {
        OrderItem *item = &table->original_table[i];
        if (item->order_id == order_id && !item->is_deleted) {
            results[j++] = item;
        }
    }
    results[j] = NULL; 

    return results;
}

void order_item_print(const OrderItem *item) {
    if (!item) return;

    printf("  Order ID: %-5u | ", item->order_id);
    printf("Product ID: %-5u | ", item->product_id);
    printf("Quantity: %-5u | ", item->quantity);
    printf("Total cost: %u", item->total_cost);

    if (item->is_deleted) printf(" [DELETED]");
    printf("\n");
}

void order_items_table_print_all(const OrderItemsTable *table) {
    if (!table) {
        if (DEBUG) printf("Something went wrong during showing all records from order_items table.\n");
        return;
    }

    int count = 0;
    for (unsigned int i = 0; i < table->size; i++) {
        if (!table->original_table[i].is_deleted) {
            order_item_print(&table->original_table[i]);
            count++;
        }
    }

    if (count == 0) printf("Order items table is empty.\n");
}

void order_items_table_print_all_by_order(const OrderItemsTable *table, const unsigned int order_id) {
    if (!table) {
        if (DEBUG) printf("Something went wrong during showing records for order_id=%u.\n", order_id);
        return;
    }

    int count = 0;
    for (unsigned int i = 0; i < table->size; i++) {
        const OrderItem *item = &table->original_table[i];
        if (item->order_id == order_id && !item->is_deleted) {
            order_item_print(item);
            count++;
        }
    }

    if (count == 0) printf("No items found for order_id=%u.\n", order_id);
}

void order_items_table_free(OrderItemsTable *table) {
    if (!table) return;

    if (table->original_table) {
        free(table->original_table);
        table->original_table = NULL;
    }

    free(table);
}