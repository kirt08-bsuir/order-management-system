#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tests.h"
#include "orders.h"
#include "product.h"
#include "order_items.h"
#include "service.h"
#include "utils.h"
#include "config.h"


typedef enum {
    STEP_MAIN           = 0,
    STEP_LOAD           = 1,
    STEP_VIEW           = 2,
    STEP_SORT           = 3,
    STEP_SEARCH         = 4,
    STEP_ADD            = 5,
    STEP_DELETE         = 6,
    STEP_EDIT           = 7,
    STEP_SF             = 8,
    STEP_EXIT_NO_SAVE   = 9,
    STEP_EXIT_SAVE      = 10,
    STEP_QUIT           = 11,
} Step;

static void print_header(const char *title) {
    system("clear");
    printf("\n==========================================================================================\n");
    printf("  %s\n", title);
    printf("==========================================================================================\n");
}

static void print_table_header(const char *table_name) {
    printf("  [ Table: %s ]\n", table_name);
    printf("------------------------------------------------------------------------------------------\n");
}

static void wait_for_enter(void) {
    printf("\n  Press Enter to continue...");
    getchar();
}


static Step handle_main(bool data_loaded) {
    print_header("ORDER MANAGEMENT SYSTEM");

    if (!data_loaded) {
        printf("  1. Load data from files\n");
        printf("  9. Exit\n");
        printf("\n  [Other menu items are available after loading data]\n");
    } else {
        printf("  1. Load data from files\n");
        printf("  2. View lists\n");
        printf("  3. Sort data\n");
        printf("  4. Search / filter\n");
        printf("  5. Add records\n");
        printf("  6. Delete records\n");
        printf("  7. Edit records\n");
        printf("  8. Invoices\n");
        printf("  9. Exit without saving\n");
        printf(" 10. Exit and save\n");
    }

    printf("\n  Your choice: ");

    char buf[8];
    input_get_string(buf, sizeof(buf));

    if (buf[0] == '\0') {
        printf("  Invalid input.\n");
        return STEP_MAIN;
    }

    if (!data_loaded) {
        if (buf[0] == '1' && buf[1] == '\0') return STEP_LOAD;
        if (buf[0] == '9' && buf[1] == '\0') return STEP_QUIT;
        printf("  Invalid input. Only options 1 and 9 are available.\n");
        return STEP_MAIN;
    }

    if (buf[0] == '1' && buf[1] == '0' && buf[2] == '\0') return STEP_EXIT_SAVE;

    switch (buf[0]) {
        case '1': return STEP_LOAD;
        case '2': return STEP_VIEW;
        case '3': return STEP_SORT;
        case '4': return STEP_SEARCH;
        case '5': return STEP_ADD;
        case '6': return STEP_DELETE;
        case '7': return STEP_EDIT;
        case '8': return STEP_SF;
        case '9': return STEP_EXIT_NO_SAVE;
        default:
            printf("  Invalid input.\n");
            return STEP_MAIN;
    }
}


static Step handle_view(
    ProductsTable *pt,
    OrdersTable *ot,
    OrderItemsTable *oit
) {
    print_header("VIEW LISTS");
    printf("  0. Back\n");
    printf("  1. Products (price list)\n");
    printf("  2. Orders\n");
    printf("  3. Order items\n");
    printf("\n  Your choice: ");

    char ch = input_get_char();

    switch (ch) {
        case '0': return STEP_MAIN;

        case '1':
            print_header("VIEW");
            print_table_header("Products (price list)");
            products_table_print_all(pt);
            wait_for_enter();
            break;

        case '2':
            print_header("VIEW");
            print_table_header("Orders");
            orders_table_print_all(ot);
            wait_for_enter();
            break;

        case '3':
            print_header("VIEW");
            print_table_header("Order items");
            order_items_table_print_all(oit);
            wait_for_enter();
            break;

        default:
            printf("  Invalid input.\n");
    }

    return STEP_VIEW;
}


static Step handle_sort(
    ProductsTable *pt,
    OrdersTable *ot
) {
    print_header("SORT DATA");
    printf("  0. Back\n");
    printf("  1. Products by name\n");
    printf("  2. Products by stock quantity\n");
    printf("  3. Orders by customer name\n");
    printf("\n  Your choice: ");

    char ch = input_get_char();

    switch (ch) {
        case '0': return STEP_MAIN;

        case '1':
            print_header("SORT");
            print_table_header("Products — by name");
            products_table_sorted_by_name(pt);
            wait_for_enter();
            break;

        case '2':
            print_header("SORT");
            print_table_header("Products — by quantity");
            products_table_sorted_by_quantity(pt);
            wait_for_enter();
            break;

        case '3':
            print_header("SORT");
            print_table_header("Orders — by customer name");
            orders_table_print_sorted_by_customer_name(ot);
            wait_for_enter();
            break;

        default:
            printf("  Invalid input.\n");
    }

    return STEP_SORT;
}


static Step handle_search(
    ProductsTable *pt,
    OrdersTable *ot
) {
    print_header("SEARCH / FILTER");
    printf("  0. Back\n");
    printf("  1. Products with stock quantity <= N\n");
    printf("  2. Find product by name\n");
    printf("  3. Find orders by customer name\n");
    printf("\n  Your choice: ");

    char ch = input_get_char();

    switch (ch) {
        case '0': return STEP_MAIN;

        case '1': {
            print_header("SEARCH");
            print_table_header("Products by quantity");
            printf("  Max quantity: ");
            unsigned int target = input_get_uint();
            service_filter_products_by_quantity(pt, target);
            wait_for_enter();
            break;
        }

        case '2': {
            print_header("SEARCH");
            print_table_header("Products by name");
            char name[PRODUCT_NAME_MAX];
            printf("  Product name: ");
            input_get_string(name, PRODUCT_NAME_MAX);
            products_table_find_by_name(pt, name);
            wait_for_enter();
            break;
        }

        case '3': {
            print_header("SEARCH");
            print_table_header("Orders by customer name");
            char name[ORDER_NAME_MAX];
            printf("  Customer name: ");
            input_get_string(name, ORDER_NAME_MAX);
            orders_table_find_by_name(ot, name);
            wait_for_enter();
            break;
        }

        default:
            printf("  Invalid input.\n");
    }

    return STEP_SEARCH;
}


static Step handle_add(
    ProductsTable *pt,
    OrdersTable *ot,
    OrderItemsTable *oit
) {
    print_header("ADD RECORDS");
    printf("  0. Back\n");
    printf("  1. Add product to price list\n");
    printf("  2. Create new order\n");
    printf("  3. Add product to existing order\n");
    printf("\n  Your choice: ");

    char ch = input_get_char();

    switch (ch) {
        case '0': return STEP_MAIN;

        case '1': {
            print_header("ADD");
            print_table_header("Products (price list)");

            char name[PRODUCT_NAME_MAX];
            printf("  Product name: ");
            input_get_string(name, PRODUCT_NAME_MAX);

            printf("  Unit price: ");
            unsigned int price = input_get_uint();

            printf("  Stock quantity: ");
            unsigned int qty = input_get_uint();

            unsigned int new_id = products_table_add(pt, name, price, qty);
            if (new_id) printf("  + Product added. ID = %u\n", new_id);
            else        printf("  ! Failed to add product.\n");

            wait_for_enter();
            break;
        }

        case '2': {
            print_header("ADD");
            print_table_header("Orders");

            char name[ORDER_NAME_MAX];
            printf("  Customer name: ");
            input_get_string(name, ORDER_NAME_MAX);

            unsigned int new_id = orders_table_add(ot, name);
            if (new_id) printf("  + Order created. ID = %u\n", new_id);
            else        printf("  ! Failed to create order.\n");

            wait_for_enter();
            break;
        }

        case '3': {
            print_header("ADD");
            print_table_header("Order items");

            printf("  Order ID (Enter `0` to cancel): ");
            unsigned int order_id = input_get_uint();

            if (order_id == 0) {
                printf("  - Operation was cancelled.\n");
                wait_for_enter();
                break;
            }

            printf("  Product ID: ");
            unsigned int product_id = input_get_uint();

            printf("  Quantity: ");
            unsigned int qty = input_get_uint();

            int res = service_add_item_to_order(pt, ot, oit, order_id, product_id, qty);
            if      (res == 0)  printf("  + Product added to order.\n");
            else if (res == -1) printf("  ! Order ID %u not found.\n", order_id);
            else if (res == -2) printf("  ! Product ID %u not found.\n", product_id);
            else if (res == -3) printf("  ! Not enough products (quantity).\n");
            else                printf("  ! Failed to add item.\n");

            wait_for_enter();
            break;
        }

        default:
            printf("  Invalid input.\n");
    }

    return STEP_ADD;
}

static Step handle_delete(
    ProductsTable *pt,
    OrdersTable *ot,
    OrderItemsTable *oit
) {
    print_header("DELETE RECORDS");
    printf("  0. Back\n");
    printf("  1. Delete product from price list\n");
    printf("  2. Delete order (and all its items)\n");
    printf("  3. Delete product from order\n");
    printf("\n  Your choice: ");

    char ch = input_get_char();

    switch (ch) {
        case '0': return STEP_MAIN;

        case '1': {
            print_header("DELETE");
            print_table_header("Products (price list)");

            printf("  Don't know the ID? Enter name to search (Enter to skip): ");
            char name[PRODUCT_NAME_MAX];
            input_get_string(name, PRODUCT_NAME_MAX);

            if (name[0] != '\0') {
                printf("\n  Matching products:\n");
                products_table_find_by_name(pt, name);
                printf("\n");
            }

            printf("  Product ID to delete (Enter `0` to cancel): ");
            unsigned int product_id = input_get_uint();

            if (product_id == 0) {
                printf("  - Operation was cancelled.\n");
                wait_for_enter();
                break;
            }

            OrderItem *item = order_items_table_find_order_with_product_id(oit, product_id);
            if (item && !item->is_deleted) {
                order_item_print(item);
                printf("  ! Operation is invalid as product already exists at least in one order.\n");
            } else {
                int res = products_table_delete_by_id(pt, product_id);
                if (res == 0)  printf("  - Product deleted.\n");
                else           printf("  ! Product ID %u not found.\n", product_id);
            }

            wait_for_enter();
            break;
        }

        case '2': {
            print_header("DELETE");
            print_table_header("Orders");

            printf("  Don't know the ID? Enter customer name to search (Enter to skip): ");
            char name[ORDER_NAME_MAX];
            input_get_string(name, ORDER_NAME_MAX);

            if (name[0] != '\0') {
                printf("\n  Matching orders:\n");
                orders_table_find_by_name(ot, name);
                printf("\n");
            }

            printf("  Order ID to delete (Enter `0` to cancel): ");
            unsigned int id = input_get_uint();

            if (id == 0) {
                printf("  - Operation was cancelled.\n");
                wait_for_enter();
                break;
            }

            int res = service_delete_order(pt, ot, oit, id);
            if (res == 0)  printf("  - Order and all its items deleted.\n");
            else if (res == -1) printf("  ! Order ID %u not found.\n", id);
            else if (res == 1) printf("   ! Somethign went wrong during operation. Try again latter.\n");

            wait_for_enter();
            break;
        }

        case '3': {
            print_header("DELETE");
            print_table_header("Order items");

            printf("  Order ID (Enter `0` to cancel): ");
            unsigned int order_id = input_get_uint();

            if (order_id == 0) {
                printf("  - Operation was cancelled.\n");
                wait_for_enter();
                break;
            }

            printf("\n  Current items in order:\n");
            order_items_table_print_all_by_order(oit, order_id);
            printf("\n");

            printf("  Product ID to remove from order: ");
            unsigned int product_id = input_get_uint();

            int res = service_delete_item_from_order(ot, oit, order_id, product_id);
            if (res == 0)  printf("  - Item deleted.\n");
            else           printf("  ! Item not found.\n");

            wait_for_enter();
            break;
        }

        default:
            printf("  Invalid input.\n");
    }

    return STEP_DELETE;
}


static Step handle_edit(
    ProductsTable *pt,
    OrdersTable *ot,
    OrderItemsTable *oit
) {
    print_header("EDIT RECORDS");
    printf("  0. Back\n");
    printf("  1. Edit product\n");
    printf("  2. Edit order\n");
    printf("  3. Edit item quantity in order\n");
    printf("\n  Your choice: ");

    char ch = input_get_char();

    switch (ch) {
        case '0': return STEP_MAIN;

        case '1': {
            print_header("EDIT");
            print_table_header("Products (price list)");

            printf("  Don't know the ID? Enter name to search (Enter to skip): ");
            char search_name[PRODUCT_NAME_MAX];
            input_get_string(search_name, PRODUCT_NAME_MAX);

            if (search_name[0] != '\0') {
                printf("\n  Matching products:\n");
                products_table_find_by_name(pt, search_name);
                printf("\n");
            }

            printf("  Product ID to edit (Enter `0` to cancel): ");
            unsigned int id = input_get_uint();

            if (id == 0) {
                printf("  - Operation was cancelled.\n");
                wait_for_enter();
                break;
            }

            char new_name[PRODUCT_NAME_MAX];
            printf("  New name (Enter to keep): ");
            input_get_string(new_name, PRODUCT_NAME_MAX);

            printf("  New price (0 to keep): ");
            unsigned int new_price = input_get_uint();

            printf("  Change quantity? (1 = yes, 0 = no): ");
            char change_qty_ch = input_get_char();
            bool change_qty = (change_qty_ch == '1');

            unsigned int new_qty = 0;
            if (change_qty) {
                printf("  New quantity: ");
                new_qty = input_get_uint();
            }

            int res = service_edit_product_record(
                pt,
                oit,
                id,
                new_name,
                new_price,
                change_qty,
                new_qty
            );

            if (res == 0)       printf("  ~ Product updated.\n");
            else if (res == 2) printf("  ! Product's record price can't be changed as it has been already included inside at least one order.\n");
            else if (res == -1) printf("  ! Product ID %u not found.\n", id);
            else                printf("  ! Edit failed.\n");

            wait_for_enter();
            break;
        }

        case '2': {
            print_header("EDIT");
            print_table_header("Orders");

            printf("  Don't know the ID? Enter customer name to search (Enter to skip): ");
            char search_name[ORDER_NAME_MAX];
            input_get_string(search_name, ORDER_NAME_MAX);

            if (search_name[0] != '\0') {
                printf("\n  Matching orders:\n");
                orders_table_find_by_name(ot, search_name);
                printf("\n");
            }

            printf("  Order ID to edit: ");
            unsigned int id = input_get_uint();

            char new_name[ORDER_NAME_MAX];
            printf("  New customer name: ");
            input_get_string(new_name, ORDER_NAME_MAX);

            int res = orders_table_edit(ot, id, new_name);
            if (res == 0)       printf("  ~ Order updated.\n");
            else if (res == -1) printf("  ! Order ID %u not found.\n", id);
            else                printf("  ! Edit failed.\n");

            wait_for_enter();
            break;
        }

        case '3': {
            print_header("EDIT");
            print_table_header("Order items");

            printf("  Order ID (Enter `0` to cancel): ");
            unsigned int order_id = input_get_uint();

            if (order_id == 0) {
                printf("  - Operation was cancelled.\n");
                wait_for_enter();
                break;
            }

            printf("\n  Current items in order:\n");
            order_items_table_print_all_by_order(oit, order_id);
            printf("\n");

            printf("  Product ID (Enter `0` to cancel): ");
            unsigned int product_id = input_get_uint();

            if (product_id == 0) {
                printf("  - Operation was cancelled.\n");
                wait_for_enter();
                break;
            }

            printf("  New quantity: ");
            unsigned int new_qty = input_get_uint();

            OrderItem *item = order_items_table_find_order_item(oit, order_id, product_id);
            if (!item) {
                printf("  ! Item not found.\n");
                wait_for_enter();
                break;
            }

            if (item->quantity == new_qty) {
                printf("  ~ Quantities are equal.\n");
                wait_for_enter();
                break;
            }
            
            int res = products_table_add_delta_to_quantity(pt, product_id, item->quantity - new_qty);
            if (res == -2) {
                printf("  ! Not enough product.\n");
                wait_for_enter();
                break;
            }

            int old_cost = (int)item->total_cost;
            order_items_table_edit_quantity(oit, order_id, product_id, new_qty);

            Product *product = products_table_search_by_id(pt, product_id);
            if (product) {
                unsigned int new_cost = new_qty * product->unit_price;
                order_items_table_set_total_cost(oit, order_id, product_id, new_cost);
                orders_table_update_total_sum(ot, order_id, (int)new_cost - old_cost);
            }

            printf("  ~ Quantity updated.\n");
            wait_for_enter();
            break;
        }

        default:
            printf("  Invalid input.\n");
    }

    return STEP_EDIT;
}

static Step handle_sf(
    ProductsTable *pt,
    OrdersTable *ot,
    OrderItemsTable *oit
) {
    print_header("SPECIAL FUNCTION — INVOICES");
    printf("  0. Back\n");
    printf("  1. Print invoice for a specific order\n");
    printf("  2. Print invoices for all orders\n");
    printf("\n  Your choice: ");
 
    char ch = input_get_char();
 
    switch (ch) {
        case '0': return STEP_MAIN;
 
        case '1': {
            printf("  Order ID: ");
            unsigned int id = input_get_uint();
 
            int res = service_print_invoice(pt, ot, oit, id);
            if (res == -1) printf("  ! Order ID %u not found.\n", id);
            else if (res == 1) printf("  ! Failed to write invoice file.\n");
 
            wait_for_enter();
            break;
        }
 
        case '2':
            service_print_all_invoices(pt, ot, oit);
            wait_for_enter();
            break;
 
        default:
            printf("  Invalid input.\n");
    }
 
    return STEP_SF;
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--test") == 0) { run_tests(); return 0; }

    ProductsTable *products_table = products_table_create();
    OrdersTable *orders_table = orders_table_create();
    OrderItemsTable *order_items_table = order_items_table_create();

    if (!products_table || !orders_table || !order_items_table) {
        fprintf(stderr, "Critical error: failed to allocate memory.\n");
        products_table_free(products_table);
        orders_table_free(orders_table);
        order_items_table_free(order_items_table);
        return 1;
    }

    bool data_loaded = false;
    Step step = STEP_MAIN;

    while (step != STEP_QUIT) {
        switch (step) {

            case STEP_MAIN:
                step = handle_main(data_loaded);
                break;

            case STEP_LOAD: {
                int res = service_load_tables(products_table, orders_table, order_items_table);
                if (res == 0) {
                    data_loaded = true;
                    printf("  + Data loaded successfully.\n");
                } else {
                    printf("  ! Load failed. Check that binary files exist.\n");
                }
                step = STEP_MAIN;
                wait_for_enter();
                break;
            }

            case STEP_VIEW:
                step = handle_view(products_table, orders_table, order_items_table);
                break;

            case STEP_SORT:
                step = handle_sort(products_table, orders_table);
                break;

            case STEP_SEARCH:
                step = handle_search(products_table, orders_table);
                break;

            case STEP_ADD:
                step = handle_add(products_table, orders_table, order_items_table);
                break;

            case STEP_DELETE:
                step = handle_delete(products_table, orders_table, order_items_table);
                break;

            case STEP_EDIT:
                step = handle_edit(products_table, orders_table, order_items_table);
                break;

            case STEP_SF:
                step = handle_sf(products_table, orders_table, order_items_table);
                break;

            case STEP_EXIT_NO_SAVE:
                printf("\n  Exiting without saving. All changes are lost.\n");
                step = STEP_QUIT;
                break;

            case STEP_EXIT_SAVE: {
                int res = service_save_tables(products_table, orders_table, order_items_table);
                if (res == 0) printf("\n  + Data saved. Goodbye.\n");
                else          printf("\n  ! Save failed.\n");
                step = STEP_QUIT;
                break;
            }

            default:
                step = STEP_MAIN;
        }
    }

    products_table_free(products_table);
    orders_table_free(orders_table);
    order_items_table_free(order_items_table);

    return 0;
}