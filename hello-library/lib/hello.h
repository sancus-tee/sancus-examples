#ifndef HELLO_H_INC
#define HELLO_H_INC

#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>

// Untrusted functions
void untrusted_hello(void);

extern struct SancusModule hello;
void SM_ENTRY(hello) hello_greet(void);
void SM_ENTRY(hello) hello_exit(void);

#endif
