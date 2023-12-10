#include "../common_lib.h"
#include "days.h"
#pragma warning(disable : 4996)

#define MAX_LINES (2000)
#define MAX_LINE_LEN (250)

static long long int Times[MAX_LINE_LEN];
static long long int Distances[MAX_LINE_LEN];
static int beat_race = 0;

static __int64 GetDistance(__int64 hold_btn_ms, __int64 step_mm_per_ms, __int64 max_time_ms);
int GetNumbersInLine_withouSpace(char* line, long long int* array);

void InitDay6() {
}
void ResultDay6() {
	printf("Number of beaten races : %d\n", beat_race);
}

int day6(bool part)
{
	FILE* fp;
	int races = 0;
	fp = fopen("day6.data", "r");
	//get seeds
	char line[MAX_LINE_LEN] = "";
	char dest[MAX_LINE_LEN] = "";

	//time
	fgets(line, MAX_LINE_LEN, fp);
	GetLine(line, dest, MAX_LINE_LEN);
	races = GetNumbersInLine_withouSpace(dest, Times);
	//distance
	fgets(line, MAX_LINE_LEN, fp);
	GetLine(line, dest, MAX_LINE_LEN);
	GetNumbersInLine_withouSpace(dest, Distances);
	//process
	for (int i = 0; i < races; i++)
	{
		__int64 h_ms = 1;//start at 1 ms hold
		__int64 d = 1;
		int temp = 0;
		while (d != 0)
		{
			d = GetDistance(h_ms, 1, Times[i]);
			if (d > Distances[i])
			{
				++beat_race;
			}
			++h_ms;
		}
	}
	return 0;
}

static __int64 GetDistance(__int64 hold_btn_ms, __int64 step_mm_per_ms, __int64 max_time_ms)
{
	if (hold_btn_ms >= max_time_ms)
		return 0;
	return (step_mm_per_ms * hold_btn_ms) * (max_time_ms - hold_btn_ms);
}


int GetNumbersInLine_withouSpace(char* line, long long int* array)
{
	int i = 0;
	int j = 0;
	long long int num = 0;
	int converted_nb_cnt = 0;
	bool flag = false;
	while (line[i] != 0)
	{
		if (IsNumber(line[i]) != -1)
		{
			num *= 10;
			num += (line[i] - 0x30);
			flag = true;
		}
		else if (line[i] != ' ')
		{
			if (flag == true)
			{
				array[j] = num;
				j++;
				num = 0;
				flag = false;
				converted_nb_cnt++;
			}
		}
		i++;
	}
	if (flag == true)
	{
		array[j] = num;
		j++;
		num = 0;
		converted_nb_cnt++;
	}
	return converted_nb_cnt;
}
