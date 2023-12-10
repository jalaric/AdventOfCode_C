#include "day_list.h"
#include <stdbool.h>
#ifndef DAYS_H
#define DAYS_H

#define X(NAME) \
int NAME(bool);
DAY_FUNCTIONS
#undef X
#define X(NAME) \
void NAME(void);
INIT_DAY_FUNCTIONS
OUTPUT_DAY_FUNCTIONS
#undef X

#define kAocDays (24)

bool runDay(int day);
void outputResult(int day);
void initDay(int day);

#endif /*DAYS_H*/