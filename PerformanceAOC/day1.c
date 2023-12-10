#include "stdio.h"
#include "string.h"
#include "days.h"
#include "../common_lib.h"
#pragma warning(disable : 4996)

static const char number_text[9][10] = {
	[0] = "one",
	[1] = "two",
	[2] = "three",
	[3] = "four",
	[4] = "five",
	[5] = "six",
	[6] = "seven",
	[7] = "eight",
	[8] = "nine"
};
static int sum = 0;
static char line[500U];

static int GetNumberInLine(char* line, size_t size, bool part);

void InitDay1()
{//empty
}

int day1(bool part)
{
	FILE* fp;
	fp = fopen("day1.data", "r");
	sum = 0;
	while (!feof(fp))
	{
		if (fgets(line, sizeof(line), fp)) {
			char* end = strchr(line, 0x0A);
			sum += GetNumberInLine(line, (end - line + 1), part);
		}
	}
	fclose(fp);
	return 0;
}

void ResultDay1()
{//empty
	printf("Calculated sum : %d \n", sum);
}

static int GetNumberInLine(char* line, size_t size, bool part)
{
	int temp = 0;
	int min_pos = _CRT_INT_MAX;
	int f = 0;
	int max_pos = -1;
	int l = 0;
	for (int i = 0; i < size; i++)
	{
		int n = IsNumber(line[i]);
		if(n != -1)
		{
			if (i < min_pos) {
				min_pos = i;
				f = n;
			}
			if (i > max_pos) {
				max_pos = i;
				l = n;
			}
		}
	}
	if (part == true){
		for (int i = 0; i < 9; i++)
		{
			char* ret = strstr(line, number_text[i]);
			while (ret != NULL)//multiple same string numbers 
			{
				if ((int)(ret - line) < min_pos) {
					min_pos = (ret - line);
					f = i + 1;
				}
				if ((int)(ret - line) >= max_pos) {
					max_pos = (ret - line);
					l = i + 1;
				}
				ret = strstr(&line[ret - line + 1], number_text[i]);
			}
		}
	}
	return (f*10 + l);
}
