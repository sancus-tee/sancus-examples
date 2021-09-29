#include <sancus/sm_support.h>
#include <stdio.h>

SM_ENTRY(sm1) void init(uint8_t* input_data, size_t len)

{
    (void) input_data;
    (void) len;
     puts("hello from sm1");
}
