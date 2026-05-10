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


// integration tests
int test_integration_add_and_rebuild_indexes(void);


// main func
void run_tests(void);

#endif