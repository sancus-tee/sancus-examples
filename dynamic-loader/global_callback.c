#include "global_callback.h"

int cb_fired = 0;

int global_callback_func(int a, int b)
{
    cb_fired++;
    return a + b;
}
