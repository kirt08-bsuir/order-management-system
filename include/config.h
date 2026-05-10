#ifndef CONFIG_H
#define CONDIG_H


#define DEBUG 1
#define DATA_FOLDER_PREFIX "data/"

// PRODUCT
#define PRODUCT_NAME_MAX 100
#define PRODUCT_INITIAL_CAPACITY 8
#define PRODUCT_GROWTH_FACTOR 1.1  // + 10% when extend
#define PRODUCT_FILE DATA_FOLDER_PREFIX "products.bin"


#endif