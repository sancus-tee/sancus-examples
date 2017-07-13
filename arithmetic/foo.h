#ifndef FOO_H_INC
#define FOO_H_INC

#include <sancus/sm_support.h>

extern struct SancusModule foo;

int SM_ENTRY(foo) enter_foo(int i);

#endif
