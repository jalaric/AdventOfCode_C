#include "common_lib.h"

int IsNumber(char c)
{
	if ((c <= 0x39) && (c >= 0x30))
	{
		return ((int)c - 0x30);
	}
	return -1;
}

void GetLine(char* buffer, char* dest, size_t size)
{
	char* begin = strchr(buffer, ':');
	char* end = strchr(buffer, 0x0A);
	size_t size_ = end - (begin)-1;
	if (size_ < size)
	{
		if (begin != NULL)
			memcpy(dest, (begin + 1), size_);
		dest[size_] = 0;
	}
}


int GetNumbersInLine(char* line, long long int* array)
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
		else
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
