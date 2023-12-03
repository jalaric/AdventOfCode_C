#include "stdio.h"
#include "string.h"
#pragma warning(disable : 4996)

#define MAX_LINES (2000)
#define MAX_LINE_LEN (200)
#define MAX_NB	(100)

static int IsNumber(char c);
static char IsSymbol(char c);
static int GetNextNumber(char* line, int* number, int* position, int*digits);
static int GetNextSymbol(char* line, char* symbol, int* position);
static int GetLineSum(struct engine_line* cur, struct engine_line* prev, struct engine_line* next);

struct engine_line
{
	int numbers[MAX_NB];
	int positions[MAX_NB][2];

	char symbol[MAX_NB];
	int position_sym[MAX_NB];
};

struct engine
{
	struct engine_line l[MAX_LINES];
};

struct engine e = { 0 };

int main()
{
	FILE* fp;
	fp = fopen(".data", "r");
	int i = 0;
	while (!feof(fp))
	{
		int nb, pos, digits, j, p_s;
		char s;
		char line[MAX_LINE_LEN] = "";
		fgets(line, MAX_LINE_LEN, fp);
		j = 0;
		while (GetNextNumber(line, &nb, &pos, &digits))
		{
			e.l[i].numbers[j] = nb; 
			e.l[i].positions[j][0] = pos;
			e.l[i].positions[j][1] = digits + pos - 1;
			++j;
		}
		j = 0;
		while (GetNextSymbol(line, &s, &p_s))
		{
			e.l[i].symbol[j] = s;
			e.l[i].position_sym[j] = p_s;
			++j;
		}
		i++;
	}
	fclose(fp);
	//IsValid Digit + Sum 31165125
	int sum = 0;
	sum += GetGearSum(&e.l[0], &e.l[MAX_LINES-1], &e.l[1]);
	for (int n = 1; n < i; n++)
	{
		sum += GetGearSum(&e.l[n], &e.l[n-1], &e.l[n+1]);
	}
	printf("Gear sum : %d\n", sum);
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
		{i++;}
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
				offset = *position+1;
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
	{
		prev = cur;
	}
	if (next == NULL)
	{
		next = cur;
	}

	while (cur->numbers[i] != 0)
	{
		int j = 0;
		//current line 
		while ((cur->symbol[j] != 0) && (flag == 0))
		{
			if ((cur->position_sym[j] <= (cur->positions[i][1] + 1)) && (cur->position_sym[j] >= (cur->positions[i][0] - 1)))
			{
				sum += cur->numbers[i];
				flag = 1;
			}
			j++;
		}
		//next line
		j = 0;
		while ((next->symbol[j] != 0) && (flag == 0))
		{
			if ((next->position_sym[j] <= (cur->positions[i][1] + 1)) && (next->position_sym[j] >= (cur->positions[i][0] - 1)))
			{
				sum += cur->numbers[i];
				flag = 1;
			}
			j++;
		}
		//previous line
		j = 0;
		while ((prev->symbol[j] != 0) && (flag == 0))
		{
			if ((prev->position_sym[j] <= (cur->positions[i][1] + 1)) && (prev->position_sym[j] >= (cur->positions[i][0] - 1)))
			{
				sum += cur->numbers[i];
				flag = 1;
			}
			j++;
		}
		flag = 0;
		i++;
	}
	return sum;
}

static int GetGearSum(struct engine_line* cur, struct engine_line* prev, struct engine_line* next)
{
	int mul = 0;
	int sum = 0;
	int i = 0;
	int flag = 0;

	if (prev == NULL)
	{
		prev = cur;
	}
	if (next == NULL)
	{
		next = cur;
	}

	while (cur->symbol[i] != 0)
	{
		if (cur->symbol[i] == '*')
		{
			//current line
			int j = 0;
			while ((flag < 2) && (cur->numbers[j] != 0))
			{
				if ((cur->position_sym[i] <= (cur->positions[j][1] + 1)) && (cur->position_sym[i] >= (cur->positions[j][0] - 1)))
				{//adj number
					if(mul == 0)
						mul = cur->numbers[j];
					else
						mul *= cur->numbers[j];
					flag++;
				}
				j++;
			}
			//next line
			j = 0;
			while ((flag < 2) && (next->numbers[j] != 0))
			{
				if ((cur->position_sym[i] <= (next->positions[j][1] + 1)) && (cur->position_sym[i] >= (next->positions[j][0] - 1)))
				{//adj number
					if (mul == 0)
						mul = next->numbers[j];
					else
						mul *= next->numbers[j];
					flag++;
				}
				j++;
			}
			//previous line
			j = 0;
			while ((flag < 2) && (prev->numbers[j] != 0))
			{
				if ((cur->position_sym[i] <= (prev->positions[j][1] + 1)) && (cur->position_sym[i] >= (prev->positions[j][0] - 1)))
				{//adj number
					if (mul == 0)
						mul = prev->numbers[j];
					else
						mul *= prev->numbers[j];
					flag++;
				}
				j++;
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