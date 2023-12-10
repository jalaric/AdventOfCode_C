#include "days.h"

#define X(NAME) NAME,
typedef int(*fp)(bool);
typedef void(*fp_v)(void);
fp daysFunctions[kAocDays+1] = {
	DAY_FUNCTIONS 0
};
fp_v initDaysFunctions[kAocDays + 1] = {
	INIT_DAY_FUNCTIONS 0
};
fp_v outputDaysFunctions[kAocDays + 1] = {
	OUTPUT_DAY_FUNCTIONS 0
};
#undef X

void initDay(int day)
{
	if (day <= kAocDays) {
		if (initDaysFunctions[day] != 0)
			initDaysFunctions[day]();
	}
}

bool runDay(int day, bool part)
{
	if (day <= kAocDays) {
		if (daysFunctions[day] != 0) {
			daysFunctions[day](part);
			return true;
		}
	}
	return false;
}

void outputResult(int day) {
	if (day <= kAocDays) {
		if (outputDaysFunctions[day] != 0)
			outputDaysFunctions[day]();
	}
}