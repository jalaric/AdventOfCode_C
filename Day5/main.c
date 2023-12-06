#include "../common_lib.h"
#pragma warning(disable : 4996)

#define MAX_LINES (2000)
#define MAX_LINES_MAP (50)
#define MAX_LINE_LEN (250)

struct Map_line
{
	long long int dest_start;
	long long int src_start;
	long long int len;
};

struct Map
{
	struct Map_line m[MAX_LINES_MAP];
};

static struct Map s_map[MAX_LINES / MAX_LINES_MAP] = { 0 };

static bool IsNewMap(char* buffer, size_t size);
static long long  int ProcessMap(struct Map* m, long long int seed);

int main()
{
	FILE* fp;
	int i_map = 0;
	fp = fopen(".data", "r");
	//get seeds
	char line[MAX_LINE_LEN] = "";
	char dest[MAX_LINE_LEN] = "";
	long long int seeds[MAX_LINE_LEN] = { 0 };
	fgets(line, MAX_LINE_LEN, fp);
	GetLine(line, dest, MAX_LINE_LEN);
	int nb_seeds = GetNumbersInLine(dest, seeds);
	int j = 0;
	while (!feof(fp))
	{
		fgets(line, MAX_LINE_LEN, fp);
		if (line[0] != '\n')
		{
			if ( IsNewMap(line, sizeof(line)) )
			{
				if (j != 0)
				{
					i_map++;
					j = 0;
				}
			}
			else
			{
				long long int array[3];
				GetNumbersInLine(line, array);
				s_map[i_map].m[j].dest_start = array[0];
				s_map[i_map].m[j].src_start = array[1];
				s_map[i_map].m[j].len = array[2];
				++j;
			}
		}
	}
	long long int min = 0x7FFFFFFFFFFFFFFF;
	//process seeds
	printf("Seed -> Location\n");
	for (int o = 0; o < nb_seeds; o+=2) //loop should be optimized or multi-threaded
	{
		for (long long int p = seeds[o]; p < (seeds[o] + seeds[o + 1]); p++) //part2 loop
		{
			//printf("%10lld ", p);
			long long int loc = p;
			//go through maps
			for (int a = 0; a < i_map + 1; a++)
			{
				loc = ProcessMap(&s_map[a], loc);
			}
			if (loc < min)
			{
				min = loc;
			}
			//printf("%10lld\n", loc);
		}
	}
	printf("Minimum location : %lld", min);
	return 0;
}

static bool IsNewMap(char* buffer, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		if (buffer[i] == ':')
		{
			return true;
		}
		else if (buffer[i] == '\n')
		{
			return false;
		}
	}
	return false;
}

static long long int ProcessMap(struct Map* m, long long int seed)
{
	int i = 0;
	while(m->m[i].len != 0)
	{
		int j = 0;
		if ((seed >= m->m[i].src_start) &&
			(seed < m->m[i].src_start + m->m[i].len))
		{
			return (m->m[i].dest_start + (seed - m->m[i].src_start));
		}
		++i;
	}
	return seed;//else seed = seed
}
