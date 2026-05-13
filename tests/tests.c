#include <stdio.h>
#include <assert.h>

#include "tests.h"


void run_tests(void) {
    printf("\n========== TESTS START ==========\n");

    printf("\n--------- (Unit) ---------\n");

    assert(test_unit_products_table_create() == 1);
    printf("[OK] test_unit_products_table_create\n");

    assert(test_unit_products_resize_capacity() == 1);
    printf("[OK] test_unit_products_resize_capacity\n");

    assert(test_unit_products_delete_non_existing_record() == 1);
    printf("[OK] test_unit_products_delete_non_existing_record\n");

    assert(test_unit_products_delete_existing_record() == 1);
    printf("[OK] test_unit_products_delete_existing_record\n");

    assert(test_unit_products_delete_existing_record_after_expanding() == 1);
    printf("[OK] test_unit_products_delete_existing_record_after_expanding\n");

    assert(test_unit_products_editing_invalid() == 1);
    printf("[OK] test_unit_products_editing_invalid\n");
    
    assert(test_unit_products_editing_valid() == 1);
    printf("[OK] test_unit_products_editing_valid\n");

    assert(test_unit_products_search_by_id_non_existing_record() == 1);
    printf("[OK] test_unit_products_search_by_id_non_existing_record\n");

    assert(test_unit_products_search_by_id_existing_record() == 1);
    printf("[OK] test_unit_products_search_by_id_existing_record\n");

    assert(test_unit_orders_table_create() == 1);
    printf("[OK] test_unit_orders_table_create\n");

    assert(test_unit_orders_table_resize_capacity() == 1);
    printf("[OK] test_unit_products_resize_capacity\n");

    assert(test_unit_orders_delete_non_existing_record() == 1);
    printf("[OK] test_unit_orders_delete_non_existing_record\n");

    assert(test_unit_orders_delete_existing_record() == 1);
    printf("[OK] test_unit_orders_delete_existing_record\n");

    assert(test_unit_orders_delete_existing_record_after_expanding() == 1);
    printf("[OK] test_unit_orders_delete_existing_record_after_expanding\n");

    assert(test_unit_orders_editing_invalid() == 1);
    printf("[OK] test_unit_orders_editing_invalid\n");

    assert(test_unit_orders_editing_valid() == 1);
    printf("[OK] test_unit_orders_editing_invalid\n");

    assert(test_unit_orders_search_by_id_non_existing_record() == 1);
    printf("[OK] test_unit_orders_search_by_id_non_existing_record\n");

    assert(test_unit_orders_search_by_id_existing_record() == 1);
    printf("[OK] test_unit_orders_search_by_id_existing_record\n");

    assert(test_unit_order_items_table_create() == 1);
    printf("[OK] test_unit_order_items_table_create\n");
 
    assert(test_unit_order_items_table_resize_capacity() == 1);
    printf("[OK] test_unit_order_items_table_resize_capacity\n");
 
    assert(test_unit_order_items_add_new_item() == 1);
    printf("[OK] test_unit_order_items_add_new_item\n");
 
    assert(test_unit_order_items_add_duplicate_pair_increases_quantity() == 1);
    printf("[OK] test_unit_order_items_add_duplicate_pair_increases_quantity\n");
 
    assert(test_unit_order_items_delete_non_existing_record() == 1);
    printf("[OK] test_unit_order_items_delete_non_existing_record\n");
 
    assert(test_unit_order_items_delete_existing_record() == 1);
    printf("[OK] test_unit_order_items_delete_existing_record\n");
 
    assert(test_unit_order_items_delete_existing_record_after_expanding() == 1);
    printf("[OK] test_unit_order_items_delete_existing_record_after_expanding\n");
 
    assert(test_unit_order_items_edit_quantity_invalid() == 1);
    printf("[OK] test_unit_order_items_edit_quantity_non_existing_record\n");
 
    assert(test_unit_order_items_edit_quantity_valid() == 1);
    printf("[OK] test_unit_order_items_edit_quantity_existing_record\n");
 
    assert(test_unit_order_items_find_by_order_id_non_existing_order() == 1);
    printf("[OK] test_unit_order_items_find_by_order_id_non_existing_order\n");
 
    assert(test_unit_order_items_find_by_order_id_existing_order() == 1);
    printf("[OK] test_unit_order_items_find_by_order_id_existing_order\n");
 
    assert(test_unit_order_items_get_total_cost_non_existing() == 1);
    printf("[OK] test_unit_order_items_get_total_cost_non_existing\n");
 
    assert(test_unit_order_items_get_total_cost_existing() == 1);
    printf("[OK] test_unit_order_items_get_total_cost_existing\n");
 
    assert(test_unit_order_items_set_total_cost_non_existing() == 1);
    printf("[OK] test_unit_order_items_set_total_cost_non_existing\n");

    printf("\n--------- (Integration) ---------\n");
    assert(test_integrity_products_save_and_load() == 1);
    printf("[OK] test_integrity_products_save_and_load\n");

    assert(test_integrity_orders_save_and_load() == 1);
    printf("[OK] test_integrity_orders_save_and_load\n");

    printf("\n===================================\n");
    printf("✅ TESTS PASSED SUCCESSFULLY!\n");
    printf("===================================\n\n");
}