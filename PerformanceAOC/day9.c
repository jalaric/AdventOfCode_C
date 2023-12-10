#include "days.h"
#include "../common_lib.h"
#include <stdbool.h>
#pragma warning(disable : 4996)


#define MAX_LINES (2000)
#define MAX_LINE_LEN (300)
#define MAX_ABLEITUNG (100)
static __int64 result = 0;

void InitDay9() {
}
void ResultDay9() {
	printf("Result : %lld\n", result);
}

static int recurseAbleitung(__int64* src, __int64 len, bool part);

int day9(bool part)
{
	FILE* fp;
	fp = fopen("day9.data", "r");
	char line[MAX_LINE_LEN] = "";
	result = 0;
	while (!feof(fp))
	{
		__int64 val[MAX_ABLEITUNG] = { 0 };
		fgets(line, MAX_LINE_LEN, fp);
		int len = GetNumbersInLine(line, val, true);
		if(part)
			result += (val[0] - recurseAbleitung(val, len, part));
		else
			result += (val[len-1] + recurseAbleitung(val, len, part) );
	}
	fclose(fp);
	return 0;
}

static int recurseAbleitung(__int64* src, __int64 len, bool part)
{
	bool ctn = false;
	__int64 ret = 0;
	__int64  abl[MAX_ABLEITUNG] = { 0 };
	for (int i = 1; i < len; i++)
	{
		abl[i - 1] = src[i] - src[i - 1];
		if (abl[i - 1])
			ctn = true;
	}
	if (ctn)
	{
		__int64 new_val = recurseAbleitung(abl, (len - 1), part);
		if(part)//calc ret
			ret = abl[0] - new_val;
		else
			ret = abl[len - 2] + new_val;
	}
	return ret;
}