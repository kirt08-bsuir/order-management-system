#include <stdio.h>

#include "config.h"
#include "service.h"


static void _write_invoice(
    FILE *out,
    const Order *order,
    OrderItem **items,
    const ProductsTable *products_table
) {
    fprintf(out, "================================================================\n");
    fprintf(out, "  INVOICE\n");
    fprintf(out, "================================================================\n");
    fprintf(out, "  Order ID  : %u\n", order->order_id);
    fprintf(out, "  Customer  : %s\n", order->customer_name);
    fprintf(out, "  Date      : %s\n", order->order_date[0] ? order->order_date : "-");
    fprintf(out, "----------------------------------------------------------------\n");
    fprintf(out, "  %-5s  %-20s  %-16s  %-8s  %s\n",
            "ID", "Name", "Quantity", "Price", "Total");
    fprintf(out, "----------------------------------------------------------------\n");
 
    unsigned int recalculated_sum = 0;
 
    for (int i = 0; items[i] != NULL; i++) {
        OrderItem *item = items[i];
 
        Product *p = products_table_search_by_id(products_table, item->product_id);
 
        if (!p) {
            fprintf(out, "  %-5u  %-20s  %-16u  %-8s  %u\n",
                    item->product_id, "[removed]",
                    item->quantity, "?", item->total_cost);
            recalculated_sum += item->total_cost;
            continue;
        }
 
        unsigned int line_total = item->quantity * p->unit_price;
        recalculated_sum += line_total;
 
        fprintf(out, "  %-5u  %-20s  %-16u  %-8u  %u\n",
                item->product_id, p->name,
                item->quantity, p->unit_price, line_total);
    }
 
    fprintf(out, "================================================================\n");
    fprintf(out, "  TOTAL: %u\n", recalculated_sum);
    fprintf(out, "================================================================\n");
}

int service_load_tables(
    ProductsTable *products_table,
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table
) {
    if (products_table_load(products_table) != 0) {
        if (DEBUG) printf("service_load_all: failed to load products.\n");
        return -1;
    }
 
    if (orders_table_load(orders_table) != 0) {
        if (DEBUG) printf("service_load_all: failed to load orders.\n");
        return -1;
    }
 
    if (order_items_table_load(order_items_table) != 0) {
        if (DEBUG) printf("service_load_all: failed to load order_items.\n");
        return -1;
    }
 
    return 0;
}

int service_save_tables(
    const ProductsTable *products_table,
    const OrdersTable *orders_table,
    const OrderItemsTable *order_items_table
) {
    if (products_table_save(products_table) != 0) {
        if (DEBUG) printf("service_save_all: failed to save products.\n");
        return -1;
    }
 
    if (orders_table_save(orders_table) != 0) {
        if (DEBUG) printf("service_save_all: failed to save orders.\n");
        return -1;
    }
 
    if (order_items_table_save(order_items_table) != 0) {
        if (DEBUG) printf("service_save_all: failed to save order_items.\n");
        return -1;
    }
 
    return 0;
}

int service_add_item_to_order(
    ProductsTable *products_table,
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table,
    const unsigned int order_id,
    const unsigned int product_id,
    const unsigned int quantity
) {
    if (!products_table || !orders_table || !order_items_table) return 1;
    
    Order *order = orders_table_find_by_id(orders_table, order_id);
    if (!order) {
        if (DEBUG) printf("service_add_item_to_order: order %u not found.\n", order_id);
        return -1;
    }

    int res = products_table_add_delta_to_quantity(products_table, product_id, -quantity);
    if (res == -3) {
        if (DEBUG) printf("service_add_item_to_order: product %u not found.\n", product_id);
        return -2;
    }
    else if (res == -2) {
        if (DEBUG) printf("service_add_item_to_order: not enough quantity for such operation\n");
        return -3;
    }

    if (order_items_table_add(order_items_table, order_id, product_id, quantity) != 0) {
        if (DEBUG) printf("service_add_item_to_order: failed to add order_item.\n");
        return 1;
    }

    OrderItem *item = order_items_table_find_order_item(order_items_table, order_id, product_id);
    if (!item) return 1;
 
    Product *product = products_table_search_by_id(products_table, product_id);
    if (!product) {
        if (DEBUG) printf("service_add_item_to_order: product %u not found.\n", product_id);
        return -2;
    }
    unsigned int new_total_cost = item->quantity * product->unit_price;
    int old_cost = item->total_cost;

    order_items_table_set_total_cost(order_items_table, order_id, product_id, new_total_cost);
 
    int delta = (int)new_total_cost - (old_cost >= 0 ? old_cost : 0);
    orders_table_update_total_sum(orders_table, order_id, delta);
 
    return 0;
}

int service_delete_item_from_order(
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table,
    const unsigned int order_id,
    const unsigned int product_id
) {
    if (!orders_table || !order_items_table) return 1;
 
    int cost = order_items_table_get_total_cost(order_items_table, order_id, product_id);
 
    if (order_items_table_delete(order_items_table, order_id, product_id) != 0) return 1;
    orders_table_update_total_sum(orders_table, order_id, -(int)cost);
 
    return 0;
}

int service_delete_order(
    ProductsTable *products_table,
    OrdersTable *orders_table,
    OrderItemsTable *order_items_table,
    const unsigned int order_id
) {
    if (!orders_table || !order_items_table) return 1;
 
    Order *order = orders_table_find_by_id(orders_table, order_id);
    if (!order) {
        if (DEBUG) printf("service_delete_order: order %u not found.\n", order_id);
        return -1;
    }
 
    OrderItem **items = order_items_table_find_by_order(order_items_table, order_id);
    if (items) {
        for (int i = 0; items[i] != NULL; i++) {
            int res = products_table_add_delta_to_quantity(products_table, items[i]->product_id, items[i]->quantity);
            if (res != 0) {
                if (DEBUG) printf("Something went wrong during recounting quantity of product inside deleting order proccess.\n");
                return 1;
            }
            order_items_table_delete(order_items_table, order_id, items[i]->product_id);
        }
        free(items);
    }
 
    orders_table_delete(orders_table, order_id);
 
    return 0;
}

int service_edit_product_record(
    ProductsTable *products_table,
    const OrderItemsTable *order_items_table,
    const unsigned int product_id,
    const char *name,
    const unsigned int unit_price,
    const bool change_quantity,
    const unsigned int quantity
) {
    if (!products_table || !order_items_table) return 1;

    if (unit_price != 0) {
        Product *p = order_items_table_find_order_with_product_id(order_items_table, product_id);
        if (p != NULL) return 2;
    }

    int res = products_table_edit_record(
        products_table,
        product_id,
        name[0] ? name : NULL,
        unit_price,
        change_quantity,
        quantity
    );

    return res;
}

void service_filter_products_by_quantity(
    ProductsTable *products_table,
    const unsigned int target
) {
    if (!products_table) return;
    products_table_filter_by_quantity(products_table, target);
}

int service_print_invoice(
    const ProductsTable *products_table,
    const OrdersTable *orders_table,
    const OrderItemsTable *order_items_table,
    const unsigned int order_id
) {
    if (!products_table || !orders_table || !order_items_table) return 1;
 
    Order *order = orders_table_find_by_id(orders_table, order_id);
    if (!order) {
        if (DEBUG) printf("service_print_invoice: order %u not found.\n", order_id);
        return -1;
    }
 
    OrderItem **items = order_items_table_find_by_order(
        (OrderItemsTable *)order_items_table, order_id);
 
    if (!items) {
        printf("  Order %u has no items.\n", order_id);
        return 0;
    }
 
    _write_invoice(stdout, order, items, products_table);
 
    char filename[64];
    snprintf(filename, sizeof(filename), DATA_FOLDER_PREFIX "invoice_%u.txt", order_id);
 
    FILE *f = fopen(filename, "w");
    if (!f) {
        if (DEBUG) printf("service_print_invoice: cannot open %s for writing.\n", filename);
        free(items);
        return 1;
    }
 
    _write_invoice(f, order, items, products_table);
    fclose(f);
 
    printf("  Invoice saved to %s\n\n", filename);
 
    free(items);
    return 0;
}

void service_print_all_invoices(
    const ProductsTable *products_table,
    const OrdersTable *orders_table,
    const OrderItemsTable *order_items_table
) {
    if (!products_table || !orders_table || !order_items_table) return;
 
    int count = 0;
    for (unsigned int i = 0; i < orders_table->size; i++) {
        Order *o = &orders_table->original_table[i];
        if (o->is_deleted) continue;
        service_print_invoice(products_table, orders_table, order_items_table, o->order_id);
        count++;
    }
 
    if (count == 0) printf("  No orders found.\n");
}