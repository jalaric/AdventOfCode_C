#include "stdio.h"
#include "string.h"
#pragma warning(disable : 4996)

#define MAX_LINES (2000)

char number_text[9][10] = {
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

int CheckIfNumber(char* buffer, size_t size, int* position, int* value);

int main()
{
	FILE* fp;
	fp = fopen("calibration.data", "r");
	char c = fgetc(fp);
	int values[MAX_LINES][2] = { 0 };
	int positions[MAX_LINES][2] = { 0 };
	char current_line[512] = "";
	int line_index = 0;
	int char_cnt = 0;
	long long int sum = 0;

	for (int i = 0; i < MAX_LINES; i++)
	{
		positions[i][0] = _CRT_INT_MAX;
		positions[i][1] = -1;
	}

	do
	{
		current_line[char_cnt] = c;
		if ((c >= 0x30) && (c <= 0x39))
		{
			if (values[line_index][0] != 0)
			{
				values[line_index][1] = (int)c - 0x30;
				positions[line_index][1] = char_cnt;
			}
			else
			{
				values[line_index][0] = (int)c - 0x30; 
				positions[line_index][0] = char_cnt;
			}
		}
		if (c == 0x0A)
		{
			//check string
			int pos;
			int val;
			current_line[char_cnt] = 0;
			while (CheckIfNumber(current_line, char_cnt, &pos, &val) == 0)
			{
				if (pos < positions[line_index][0])
				{
					if (values[line_index][1] == 0)
					{
						positions[line_index][1] = positions[line_index][0];
						values[line_index][1] = values[line_index][0];

					}
					positions[line_index][0] = pos;
					values[line_index][0] = val;
				}
				else if (pos > positions[line_index][1])
				{
					positions[line_index][1] = pos;
					values[line_index][1] = val;
				}
			}
			memset(current_line, 0, sizeof(current_line));
			line_index++;
			char_cnt=0;
		}
		else
		{
			char_cnt++;
		}
		c = fgetc(fp);
	} while ((!feof(fp)) && (line_index < MAX_LINES));
	for (int j = 0; j < line_index+1; j++)
	{
		if (values[j][1] == 0)
		{
			values[j][1] = values[j][0];
		}
		sum += (long long int)(((long long int)values[j][0] * 10) + (long long int)values[j][1]);
	}
	printf("Calculated sum : %d \n", sum);

	fclose(fp);
	return 0;
}

int CheckIfNumber(char* buffer, size_t size, int* position, int* value)
{
	static int offset = 0;
	char string[MAX_LINES];
	int temp;
	memcpy(string, buffer, size);
	//string[size] = 0;
	*position = _CRT_INT_MAX;
	for (int i = 0; i < 9; i++)
	{
		char* ret = strstr(&string[offset], number_text[i]);
		if (ret != NULL)
		{
			if( (ret-string) < *position)
			{
				*position = (int)(ret - string);
				*value = (i + 1); 
			}
		}
	}
	if (*position != _CRT_INT_MAX)
	{
		offset = *position + 1;
		return 0;
	}
	*position = 0;
	offset = 0;
	return -1;
}
