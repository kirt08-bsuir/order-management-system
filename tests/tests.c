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

    printf("\n--------- (Integration) ---------\n");
    assert(test_integrity_orders_save_and_load() == 1);
    printf("[OK] test_integrity_orders_save_and_load\n");

    printf("\n===================================\n");
    printf("✅ TESTS PASSED SUCCESSFULLY!\n");
    printf("===================================\n\n");
}