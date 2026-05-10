#ifndef TESTS_H
#define TESTS_H


// unit tests
int test_unit_table_create(void);
int test_unit_resize_capacity(void);


// integration tests
int test_integration_add_and_rebuild_indexes(void);


// main func
void run_tests(void);

#endif