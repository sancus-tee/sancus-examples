#ifndef BAR_H_INC
#define BAR_H_INC

#include <sancus/sm_support.h>

extern struct SancusModule bar;

int SM_ENTRY(bar) bar_lookup( int idx );

#endif
