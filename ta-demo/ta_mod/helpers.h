#ifndef __TA_HELPERS_H
#define __TA_HELPERS_H

#include <string.h>


void uart2_write_byte(uint8_t c);

int putchar(int c);

int puts_n(char *s);

void puthex_byte(uint8_t v);

void puthex(uint8_t *v, int n);


#endif

