#ifndef TESTS_H
#define TESTS_H


// unit tests
int test_unit_products_table_create(void);
int test_unit_products_resize_capacity(void);
int test_unit_products_delete_non_existing_record(void);
int test_unit_products_delete_existing_record(void);
int test_unit_products_delete_existing_record_after_expanding(void);
int test_unit_products_editing_invalid(void);
int test_unit_products_editing_valid(void);
int test_unit_products_search_by_id_non_existing_record(void);
int test_unit_products_search_by_id_existing_record(void);

int test_unit_orders_table_create(void);
int test_unit_orders_table_resize_capacity(void);
int test_unit_orders_delete_non_existing_record(void);
int test_unit_orders_delete_existing_record(void);
int test_unit_orders_delete_existing_record_after_expanding(void);
int test_unit_orders_editing_invalid(void);
int test_unit_orders_editing_valid(void);
int test_unit_orders_search_by_id_non_existing_record(void);
int test_unit_orders_search_by_id_existing_record(void);

int test_unit_order_items_table_create(void);
int test_unit_order_items_table_resize_capacity(void);
int test_unit_order_items_add_new_item(void);
int test_unit_order_items_add_duplicate_pair_increases_quantity(void);
int test_unit_order_items_delete_non_existing_record(void);
int test_unit_order_items_delete_existing_record(void);
int test_unit_order_items_delete_existing_record_after_expanding(void);
int test_unit_order_items_edit_quantity_invalid(void);
int test_unit_order_items_edit_quantity_valid(void);
int test_unit_order_items_find_by_order_id_non_existing_order(void);
int test_unit_order_items_find_by_order_id_existing_order(void);
int test_unit_order_items_get_total_cost_non_existing(void);
int test_unit_order_items_get_total_cost_existing(void);
int test_unit_order_items_set_total_cost_non_existing(void);

// integration tests
int test_integrity_products_save_and_load(void);
int test_integrity_orders_save_and_load(void);

// main func
void run_tests(void);

#endif