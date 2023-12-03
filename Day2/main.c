#include "stdio.h"
#include "string.h"
#pragma warning(disable : 4996)

#define MAX_LINES (2000)
#define MAX_LINE_LEN (200)
#define COLORS_NB (3)

char colors_text[COLORS_NB][10] = {
	[0] = "green",
	[1] = "blue",
	[2] = "red"
};

enum color
{
	ENone = 0,
	Egreen,
	Eblue,
	Ered
};

struct throw_vars
{
	enum color color[10];
	int cube_number[10];
};
struct game_vars
{
	struct throw_vars t[10];
};
char* param[MAX_LINE_LEN];
struct game_vars vars[MAX_LINES];

static void GetLine(char* buffer, char* dest, size_t size);
static int GetGameLine(char* buffer, char* dest, size_t size);
static int GetCubeColorAndNumber(char* line, char separator, enum color* color, int* number);

static int GetMaxCountColorGame(struct game_vars* g, enum color clr);
static int IsGamePossible(struct game_vars* g, int blue_max, int red_max, int green_max);

int main()
{
	int blue_cubes, green_cubes, red_cubes;	
	printf("Number of green cubes : ");
	scanf("%d", &green_cubes);
	printf("Number of blue cubes : ");
	scanf("%d", &blue_cubes);
	printf("Number of red cubes : ");
	scanf("%d", &red_cubes);
	{
		FILE* fp;
		fp = fopen(".data", "r");
		int count_game = 0;
		int count_separator = 0;
		int count_throw = 0;
		while (!feof(fp))
		{
			char line[MAX_LINE_LEN] = "";
			char dest[MAX_LINE_LEN] = "";
			char game[MAX_LINE_LEN] = "";
			fgets(line, MAX_LINE_LEN, fp);
			GetLine(line, dest, MAX_LINE_LEN);
			while (GetGameLine(dest, game, MAX_LINE_LEN))
			{
				while (GetCubeColorAndNumber(game, ',', &vars[count_game].t[count_throw].color[count_separator], &vars[count_game].t[count_throw].cube_number[count_separator]))
				{
					count_separator++;
				}
				count_separator = 0;
				count_throw++;
			}
			count_throw = 0;
			count_game++;
		}
		//check impossible
		int sum = 0;
		long long int pow = 0;
		for (int i = 0; i < count_game-1; i++)
		{
			int b, r, g;
			b = GetMaxCountColorGame(&vars[i], Eblue);
			g = GetMaxCountColorGame(&vars[i], Egreen);
			r = GetMaxCountColorGame(&vars[i], Ered);
			pow += (b * r * g);
			if (IsGamePossible(&vars[i], blue_cubes, red_cubes, green_cubes) != 0)
			{
				printf("Game %d is impossible\n", i+1);
			}
			else
			{
				sum += (i + 1);
			}
		}
		printf("Sum : %d\n", sum);
		printf("Pow : %d\n", pow);
		fclose(fp);
	}
	while (1);
	return 0;
}

static int GetMaxCountColorGame(struct game_vars* g, enum color clr)
{
	int j = 0;
	int max = 0;
	for (int i = 0; i < 10; i++)
	{
		int temp = 0;
		while (g->t[i].color[j] != ENone)
		{
			if (g->t[i].color[j] == clr)
			{
				temp += g->t[i].cube_number[j];
			}
			j++;
		}
		j = 0;
		if (temp > max)
		{
			max = temp;
		}
	}
	return max;
}

static int IsGamePossible(struct game_vars* g, int blue_max, int red_max, int green_max)
{
	if (GetMaxCountColorGame(g, Egreen) > green_max)
		return -1;
	if (GetMaxCountColorGame(g, Ered) > red_max)
		return -1;
	if (GetMaxCountColorGame(g, Eblue) > blue_max)
		return -1;
	return 0;
}

static void GetLine(char* buffer, char* dest, size_t size)
{
	char* begin = strchr(buffer, ':');
	char* end = strchr(buffer, 0x0A);
	size_t size_game = end - (begin);
	memcpy(dest, (begin+1), size_game);
	dest[size_game-1] = 0;
}

static int GetGameLine(char* buffer, char* dest,  size_t size)
{
	static int offset = 0;
	static int flag = 0;
	char* begin = &buffer[offset];
	char* end = strchr(begin, ';');
	memset(dest, 0, size);
	if (end != NULL)
	{
		size_t size_game = end - (begin);
		memcpy(dest, begin, size_game);
		dest[size_game] = ',';
		dest[size_game + 1] = 0;
		offset += size_game + 1;
		return 1;
	}
	else if(flag == 0)
	{
		end = strchr(buffer, 0x00);
		size_t size_game = end - (begin);
		memcpy(dest, begin, size_game);
		dest[size_game] = ',';
		dest[size_game+1] = 0;
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

static int GetCubeColorAndNumber(char* line, char separator, enum color* color, int* number)
{
	static int offset = 0;
	char* begin = (line + offset);
	char* end = strchr(begin, separator);
	if (end != NULL)
	{
		size_t size = end - begin;
		int number_sum = 0;
		char search[20];
		memcpy(search, begin, size);
		search[size] = 0;
		for (int i = 0; i < COLORS_NB; i++)
		{
			char* ret = strstr(search, colors_text[i]);
			if (ret != NULL)
			{
				*color = i+1;
			}
		}
		for (int i = 0; i < size; i++)
		{
			char c = search[i];
			if (c > 0x29 && c < 0x40)
			{
				number_sum *= 10;
				number_sum += (int)(c-0x30);
			}
		}
		*number = number_sum;
		//line = end+1;
		offset = end + 1 - line;
		return 1;
	}
	offset = 0;
	return 0;
}