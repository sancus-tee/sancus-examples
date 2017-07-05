
#include "hello.h"
#include <stdio.h>


static void printf_wrapper(char *s)
{
    printf ("%s", s);
}



DECLARE_SM(hello, 0x1234);

static void SM_FUNC(hello) internal_func(void)
{
    printf_wrapper("hello world\n");
}

void hello_entry_fn(void)
{
    internal_func();
}

