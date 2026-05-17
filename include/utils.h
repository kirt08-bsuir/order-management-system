#ifndef UTILS_H
#define UTILS_H


bool validate_non_empty_string(const char *str);

bool validate_price(const unsigned int price);

char input_get_char(void);
void input_get_string(char *buf, int max_len);
unsigned int input_get_uint(void);

#endif