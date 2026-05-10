#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "base.h"
#include "utils.h"
#include "config.h"
#include "product.h"


static int _products_resize(ProductsTable *products_table) {
    if (!products_table) return -1;

    unsigned int new_capacity = (unsigned int)(products_table->capacity * PRODUCT_GROWTH_FACTOR);
    if (new_capacity == products_table->capacity) new_capacity++;

    Product *new_table = (Product *)realloc(products_table->original_table, new_capacity * sizeof(Product));
    if (!new_table) {
        if (DEBUG) printf("Something went wrong during allocation new memory for ProductsTable->original_table with number of records %d.\n", new_capacity);
        return -1;
    }
    products_table->original_table = new_table;

    SortedIndex *new_name_idx = (SortedIndex *)realloc(products_table->sorted_by_name, new_capacity * sizeof(SortedIndex));
    if (!new_name_idx) {
        if (DEBUG) printf("Something went wrong during allocation new memory for ProductsTable->sorted_by_name with number of records %d.\n", new_capacity);
        return -1;
    }
    products_table->sorted_by_name = new_name_idx;

    SortedIndex *new_quantity_idx = (SortedIndex *)realloc(products_table->sorted_by_quantity, new_capacity * sizeof(SortedIndex));
    if (!new_quantity_idx) {
        if (DEBUG) printf("Something went wrong during allocation new memory for ProductsTable->sorted_by_quantity with number of records %d.\n", new_capacity);
        return -1;
    }
    products_table->sorted_by_quantity = new_quantity_idx;

    products_table->capacity = new_capacity;
    return 0;
}

static int _names_comporator(const char *s1, const char *s2) {
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

static void _products_rebuild_index_by_name(ProductsTable *products_table) {
    if (!products_table || products_table->size == 0) return;
    
    for (unsigned int i = 0; i < products_table->size; i++) products_table->sorted_by_name[i].idx = i;

    for (unsigned int i = 0; i < products_table->size - 1; i++) {
        for (unsigned int j = i + 1; j < products_table->size; j++) {
            unsigned int idx1 = products_table->sorted_by_name[i].idx;
            unsigned int idx2 = products_table->sorted_by_name[j].idx;

            if (_names_comporator(
                products_table->original_table[idx1].name,
                products_table->original_table[idx2].name    
            ) > 0) {
                SortedIndex tmp = products_table->sorted_by_name[i];
                products_table->sorted_by_name[i] = products_table->sorted_by_name[j];
                products_table->sorted_by_name[j] = tmp;
            }
        }
    }
}

static void _products_rebuild_index_by_quantity(ProductsTable *products_table) {
        if (!products_table || products_table->size == 0) return;
 
    for (unsigned int i = 0; i < products_table->size; i++) {
        products_table->sorted_by_quantity[i].idx = i;
    }
 
    for (unsigned int i = 0; i < products_table->size - 1; i++) {
        for (unsigned int j = i + 1; j < products_table->size; j++) {
            unsigned int idx1 = products_table->sorted_by_quantity[i].idx;
            unsigned int idx2 = products_table->sorted_by_quantity[j].idx;
 
            if (products_table->original_table[idx1].quantity > products_table->original_table[idx2].quantity) {
                SortedIndex tmp = products_table->sorted_by_quantity[i];
                products_table->sorted_by_quantity[i] = products_table->sorted_by_quantity[j];
                products_table->sorted_by_quantity[j] = tmp;
            }
        }
    }
}

ProductsTable *products_table_create() {
    ProductsTable *products_table = (ProductsTable *)malloc(sizeof(ProductsTable));
    if (!products_table) {
        if (DEBUG) printf("Something went wrong during allocation memory for ProductsTable.\n");
        return NULL;
    }

    products_table->original_table = (Product *)malloc(PRODUCT_INITIAL_CAPACITY * sizeof(Product));
    if (!products_table->original_table) {
        if (DEBUG) printf("Something went wrong during allocation memory for ProductsTable->original_table with number of records %d.\n", PRODUCT_INITIAL_CAPACITY);
        free(products_table);
        return NULL;
    }

    products_table->sorted_by_name = (SortedIndex *)malloc(PRODUCT_INITIAL_CAPACITY * sizeof(SortedIndex));
    if (!products_table->sorted_by_name) {
        if (DEBUG) printf("Something went wrong during allocation memory for products_table->sorted_by_name with number of records %d.\n", PRODUCT_INITIAL_CAPACITY);
        free(products_table->original_table);
        free(products_table);
        return NULL;
    }

    products_table->sorted_by_quantity = (SortedIndex *)malloc(PRODUCT_INITIAL_CAPACITY * sizeof(SortedIndex));
    if (!products_table->sorted_by_quantity) {
        if (DEBUG) printf("Something went wrong during allocation memory for products_table->sorted_by_quantity with number of records %d.\n", PRODUCT_INITIAL_CAPACITY);
        free(products_table->sorted_by_name);
        free(products_table->original_table);
        free(products_table);
        return NULL;
    }

    products_table->capacity = PRODUCT_INITIAL_CAPACITY;
    products_table->size = 0;
    products_table->last_id = 0;

    return products_table;
}

unsigned int products_table_add(
    ProductsTable *products_table,
    const char *name,
    const unsigned int unit_price,
    const unsigned int quantity
) {
    if (!products_table) return 0;

    if (!validate_non_empty_string(name)) {
        printf("Validation Error: invalid value for field `name`.\n");
        return 0;
    }

    if (!validate_price(unit_price)) {
        printf("Validation Error: invalid value for field `unit_price`.\n");
        return 0;
    }

    if (products_table->size >= products_table->capacity) {
        if (_products_resize(products_table) != 0) return 0;
    }

    unsigned int new_id = ++(products_table->last_id);

    Product *p = &products_table->original_table[products_table->size];

    p->product_id = new_id; 
    p->unit_price = unit_price;
    p->quantity = quantity;
    p->is_deleted = false;

    strncpy(p->name, name, PRODUCT_NAME_MAX - 1);
    p->name[PRODUCT_NAME_MAX - 1] = '\0';
 
    products_table->size++;

    _products_rebuild_index_by_name(products_table);
    _products_rebuild_index_by_quantity(products_table);
    return new_id;
}

void product_print(const Product *p) {
    if (!p) return;
 
    printf("  ID: %-5u | ", p->product_id);
    printf("Name: %-30s | ", p->name);
    printf("Price: %-5u | ", p->unit_price);
    printf("Quantity: %u", p->quantity);
 
    if (p->is_deleted) {
        printf(" [DELETED]");
    }
    printf("\n");
}

void products_table_print_all(const ProductsTable *products_table) {
    if (!products_table) {
        if (DEBUG) printf("Something went wrong during showing all records from products table.\n");
        return;
    }
    int count = 0;
    for (unsigned int i = 0; i < products_table->size; i++) {
            if (!products_table->original_table[i].is_deleted) {
            product_print(&products_table->original_table[i]);
            count++;
        }
    }
    if (count == 0) printf("Products table is empty.\n");
}

void products_table_free(ProductsTable *products_table) {
    if (!products_table) return;

    if (products_table->sorted_by_name) {
        free(products_table->sorted_by_name);
        products_table->sorted_by_name = NULL;
    }

    if (products_table->sorted_by_quantity) {
        free(products_table->sorted_by_quantity);
        products_table->sorted_by_quantity = NULL;
    }

    if (products_table->original_table) {
        free(products_table->original_table);
        products_table->original_table = NULL;
    }

    free(products_table);
}