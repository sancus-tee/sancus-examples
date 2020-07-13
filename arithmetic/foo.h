#ifndef FOO_H_INC
#define FOO_H_INC

#include <sancus/sm_support.h>

extern struct SancusModule foo;

int SM_ENTRY(foo) enter_foo(int i);
int SM_ENTRY(foo) foo_mul(int a, int b, int c);

#endif
