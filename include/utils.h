#pragma once

#include "common.h"

void delay(u64 ticks);
void put32(u64 address, u32 value);
unsigned int get32(u64 address);
u32 get_el();