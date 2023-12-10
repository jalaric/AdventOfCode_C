#include "stdio.h"
#include "string.h"
#include "days.h"
#pragma warning(disable : 4996)

#define MAX_LINE_LEN (200)
#define MAX_NB	(100)

static int IsNumber(char c);
static char IsSymbol(char c);
static int GetNextNumber(char* line, int* number, int* position, int* digits);
static int GetNextSymbol(char* line, char* symbol, int* position);
static int GetLineSum(struct engine_line* cur, struct engine_line* prev, struct engine_line* next);
static int GetGearMul(struct engine_line* cur, struct engine_line* prev, struct engine_line* next);

struct engine_line
{
	int numbers[MAX_NB];
	int positions[MAX_NB][2];

	char symbol[MAX_NB];
	int position_sym[MAX_NB];
};

struct engine
{
	struct engine_line l[3];
};

static int sum = 0;

void InitDay3()
{
}

void ResultDay3() {
	printf("Gear sum : %d\n", sum);
}

int day3(bool part)
{
	FILE* fp;
	fp = fopen("day3.data", "r");
	int i = 0;
	int m = 0;
	sum = 0;
	struct engine e = { 0 };
	while (!feof(fp))
	{
		int nb, pos, digits, j, p_s;
		char s;
		char line[MAX_LINE_LEN] = "";
		fgets(line, MAX_LINE_LEN, fp);
		j = 0;
		e.l[m].numbers[j] = 0;
		while (GetNextNumber(line, &nb, &pos, &digits))
		{
			e.l[m].numbers[j] = nb;
			e.l[m].positions[j][0] = pos;
			e.l[m].positions[j][1] = digits + pos - 1;
			++j;
			e.l[m].numbers[j] = 0;
		}
		j = 0;
		e.l[m].symbol[j] = 0;
		while (GetNextSymbol(line, &s, &p_s))
		{
			e.l[m].symbol[j] = s;
			e.l[m].position_sym[j] = p_s;
			++j;
			e.l[m].symbol[j] = 0;
		}
		if (i > 0)
		{
			//IsValid Digit + Sum
			if (part == true)
				sum += GetGearMul(&e.l[(m + 2) % 3], &e.l[(m + 1) % 3], &e.l[m]);
			else
				sum += GetLineSum(&e.l[(m + 2) % 3], &e.l[(m + 1) % 3], &e.l[m]);
		}
		i++;
		m = i % 3;
	}
	if (part == true)
		sum += GetGearMul(&e.l[(m + 1) % 3], &e.l[(m + 2) % 3], NULL);
	else
		sum += GetLineSum(&e.l[(m + 1) % 3], &e.l[(m + 2) % 3], NULL);
	fclose(fp);
	return 0;
}

static int GetNextNumber(char* line, int* number, int* position, int* digits)
{
	static int offset = 0;
	char* begin = (line + offset);
	char* end = strchr(begin, '\n');
	if (end != NULL)
	{
		size_t size = end - begin;
		int number_sum = 0;
		int i = 0;
		int j = 0;
		*digits = 0;
		char search[MAX_LINE_LEN];
		memcpy(search, begin, size);
		while ((IsNumber(search[i]) == -1) && i < size)
		{
			i++;
		}
		if (i < size)
		{
			*position = i + offset;
			j = IsNumber(search[i]);
			while (j != -1)
			{
				number_sum *= 10;
				number_sum += j;
				*digits += 1;
				i++;
				j = IsNumber(search[i]);
			}
			*number = number_sum;
			offset = *position + *digits;
			return 1;
		}
		else
		{
			offset = 0;
			return 0;
		}
	}
}
static int GetNextSymbol(char* line, char* symbol, int* position)
{
	static int offset = 0;
	char* begin = (line + offset);
	char* end = strchr(begin, '\n');
	if (end != NULL)
	{
		size_t size = end - begin + 1;
		char search[MAX_LINE_LEN];
		memcpy(search, begin, size);
		int i = 0;
		while (i < size)
		{
			char sym = IsSymbol(search[i]);
			if (sym != 0)
			{
				*symbol = sym;
				*position = i + offset;
				offset = *position + 1;
				return 1;
			}
			i++;
		}
		offset = 0;
		return 0;
	}
}

static int IsNumber(char c)
{
	if ((c <= 0x39) && (c >= 0x30))
	{
		return ((int)c - 0x30);
	}
	return -1;
}

static char IsSymbol(char c)
{
	if (IsNumber(c) == -1)
	{
		if ((c != '.') && (c != '\n'))
		{
			return c;
		}
	}
	return 0;
}

static int GetLineSum(struct engine_line* cur, struct engine_line* prev, struct engine_line* next)
{
	int sum = 0;
	int i = 0;
	int flag = 0;
	if (prev == NULL)
		prev = cur;
	if (next == NULL)
		next = cur;
	struct engine_line* p[3] = { cur, prev, next };
	while (cur->numbers[i] != 0)
	{
		for (int x = 0; x < 3; x++) {
			int j = 0;
			while ((p[x]->symbol[j] != 0) && (flag == 0))
			{
				int temp1 = (cur->positions[i][1] + 1);
				int temp2 = (cur->positions[i][0] - 1);
				if ((p[x]->position_sym[j] <= temp1) 
					&& (p[x]->position_sym[j] >= temp2))
				{
					sum += cur->numbers[i];
					flag = 1;
				}
				j++;
			}
		}
		flag = 0;
		i++;
	}
	return sum;
}

static int GetGearMul(struct engine_line* cur, struct engine_line* prev, struct engine_line* next)
{
	int mul = 0;
	int sum = 0;
	int i = 0;
	int flag = 0;
	if (prev == NULL)
		prev = cur;
	if (next == NULL)
		next = cur;
	struct engine_line* p[3] = { cur, prev, next };
	while (cur->symbol[i] != 0)
	{
		if (cur->symbol[i] == '*')
		{
			for (int x = 0; x < 3; x++)
			{
				int j = 0;
				while ((flag < 2) && (p[x]->numbers[j] != 0))
				{
					if ((cur->position_sym[i] <= (p[x]->positions[j][1] + 1)) && (cur->position_sym[i] >= (p[x]->positions[j][0] - 1)))
					{//adj number
						if (mul == 0)
							mul = p[x]->numbers[j];
						else
							mul *= p[x]->numbers[j];
						flag++;
					}
					j++;
				}
			}
		}
		if (flag > 1)
		{
			sum += mul;
		}
		flag = 0;
		mul = 0;
		i++;
	}
	return sum;
}