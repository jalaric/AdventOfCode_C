#include "stdio.h"
#include "string.h"
#include "days.h"
#pragma warning(disable : 4996)

#define MAX_LINES (2000)
#define MAX_LINE_LEN (200)

struct line
{
	short int win_num[MAX_LINE_LEN];
	short int scratch_num[MAX_LINE_LEN];
};

int card_coef[MAX_LINES];

static int IsNumber(char c);
static void GetLine(char* buffer, char* dest, size_t size);
static int GetGameLine(char* buffer, char* dest, char separator, size_t size);
static int GetNumbers(char* buffer, short int* array);
static int GetScoreLine(short int* array, short int* win);

static int score = 0;
static int games = 0;

void InitDay4(){
}
void ResultDay4() {
	printf("Score : %d\n", score);
	printf("Games : %d\n", games);
}

int day4(bool part)
{
	FILE* fp;
	struct line l = { 0 };
	fp = fopen("day4.data", "r");
	int count_separator = 0;
	int i = 0;
	for (int a = 0; a < MAX_LINES; a++)
		card_coef[a] = 1;
	games = 0;
	score = 0;
	while (!feof(fp))
	{
		char line[MAX_LINE_LEN] = "";
		char dest[MAX_LINE_LEN] = "";
		char game[MAX_LINE_LEN] = "";
		fgets(line, MAX_LINE_LEN, fp);
		GetLine(line, dest, MAX_LINE_LEN);
		count_separator = 0;
		while (GetGameLine(dest, game, '|', MAX_LINE_LEN))
		{
			if (count_separator == 0)
				GetNumbers(game, l.win_num);
			else
				GetNumbers(game, l.scratch_num);
			count_separator++;
		}
		int temp = GetScoreLine(l.scratch_num, l.win_num, part);
		if (part) {
			for (int x = 1; x < card_coef[i] + 1; x++)
			{
				for (int b = 1; b < temp + 1; b++)
				{
					card_coef[i + b]++;
				}
			}
			games += card_coef[i];
		}
		else
		{
			score += temp;
		}
		for (int d = 0; d < MAX_LINE_LEN; d++)
		{
			l.win_num[d] = 0;
			l.scratch_num[d] = 0;
		}
		i++;
	}
	games--;
	fclose(fp);
	return 0;
}


static int IsNumber(char c)
{
	if ((c <= 0x39) && (c >= 0x30))
	{
		return ((int)c - 0x30);
	}
	return -1;
}


static int GetNumbers(char* buffer, short int* array)
{
	int i = 0;
	int j = 0;
	int num = 0;
	char digits[10] = { 0 };
	while (buffer[i] != 0)
	{
		if (IsNumber(buffer[i]) != -1)
		{
			num *= 10;
			num += (buffer[i] - 0x30);

		}
		else
		{
			if (num > 0)
			{
				array[j] = num;
				j++;
				num = 0;
			}
		}
		i++;
	}
	if (num > 0)
	{
		array[j] = num;
		j++;
		num = 0;
	}
	return 0;
}

static int GetScoreLine(short int* array, short int* win, bool part)
{
	int score = 0;
	for (int i = 0; i < MAX_LINE_LEN; i++)
	{
		for (int j = 0; j < MAX_LINE_LEN; j++)
		{
			if (array[i] == win[j])
			{
				if (array[i] != 0)
				{
					if (score == 0)
					{
						score = 1;
					}
					else
					{
						if(!part)
							score *= 2; //part1
						else
							score++; //part 2
					}
				}
			}
		}
	}
	return score;
}

static int GetGameLine(char* buffer, char* dest, char separator, size_t size)
{
	static int offset = 0;
	static int flag = 0;
	char* begin = &buffer[offset];
	char* end = strchr(begin, separator);
	if (end != NULL)
	{
		size_t size_game = end - (begin);
		memcpy(dest, begin, size_game);
		dest[size_game] = 0;
		offset += size_game + 1;
		return 1;
	}
	else if (flag == 0)
	{
		end = strchr(buffer, 0x00);
		size_t size_game = end - (begin);
		memcpy(dest, begin, size_game);
		dest[size_game] = 0;
		flag = 1;
		return 1;
	}
	else
	{
		flag = 0;
		offset = 0;
	}
	return 0;
}

static void GetLine(char* buffer, char* dest, size_t size)
{
	char* begin = strchr(buffer, ':');
	char* end = strchr(buffer, 0x0A);
	size_t size_game = end - (begin)-1;
	if (begin != NULL)
		memcpy(dest, (begin + 1), size_game);
}
