#include "../common_lib.h"
#include "days.h"
#pragma warning(disable : 4996)

#define MAX_LINES (2000)
#define MAX_LINES_MAP (250)
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
	int nb_ranges;
};

struct Seed
{
	long long int seed_start;
	long long int seed_len;
};

static struct Map s_map[MAX_LINES / MAX_LINES_MAP] = { 0 };
long long int min = 0x7FFFFFFFFFFFFFFF;

static bool IsNewMap(char* buffer, size_t size);
static long long  int ProcessMap(struct Map* m, long long int seed);
static void mergeInterval1BelowInterval2(struct Map_line* m1, struct Map_line* m2, struct Map* merged);
static void mergeInterval1UpperInterval2(struct Map_line* m1, struct Map_line* m2, struct Map* merged);
static void MergeMap(struct Map* m1, struct Map* m2, struct Map* mM);

void InitDay5() {
}
void ResultDay5() {
	printf("Minimum location : %lld\n", min);
}

int day5(bool part)
{
	FILE* fp;
	int i_map = 0;
	fp = fopen("day5.data", "r");
	//get seeds
	char line[MAX_LINE_LEN] = "";
	char dest[MAX_LINE_LEN] = "";
	long long int seeds[MAX_LINE_LEN] = { 0 };
	fgets(line, MAX_LINE_LEN, fp);
	GetLine(line, dest, MAX_LINE_LEN);
	int nb_seeds = GetNumbersInLine(dest, seeds, false);
	int j = 0;
	while (!feof(fp))
	{
		fgets(line, MAX_LINE_LEN, fp);
		if (line[0] != '\n')
		{
			if (IsNewMap(line, sizeof(line)))
			{
				if (j != 0)
				{
					s_map[i_map].nb_ranges = j;
					i_map++;
					j = 0;
				}
			}
			else
			{
				long long int array[3];
				GetNumbersInLine(line, array, false);
				s_map[i_map].m[j].dest_start = array[0];
				s_map[i_map].m[j].src_start = array[1];
				s_map[i_map].m[j].len = array[2];
				++j;
			}
		}
	}
	//process seeds
	//printf("Seed -> Location\n");
	for (int o = 0; o < nb_seeds; o += 2) //loop should be optimized or multi-threaded
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
	while (m->m[i].len != 0)
	{
		int j = 0;
		if ((seed >= m->m[i].src_start) &&
			(seed < m->m[i].src_start + m->m[i].len))
		{
			return (m->m[i].dest_start + (seed - m->m[i].src_start));
		}
		++i;
	}
	return seed;
}

/*static void processRangeSeed(struct Map* m, struct Seed* seed, struct Seed* outIntervals)
{
	int i = 0;
	while (m->m[i].len != 0)
	{
		if ((seed->seed_start < m->m[i].src_start + m->m[i].len)
			&& (seed->seed_start >= m->m[i].src_start))
		{
			seed->seed_start = (m->m[i].dest_start + (seed->seed_start - m->m[i].src_start));
			if ((seed->seed_start + seed->seed_len) > (seed->seed_start + m->m[i].len))
			{
				seed->seed_len = m->m[i].len;
				seed->
			}
			seed->intervals++;
		}
		i++;
	}
}
*/

/*
static void MergeMap(struct Map* m1, struct Map* m2, struct Map* mM)
{
	int o = 0;
	int i = 0;
	while (m1->m[i].len != 0)
	{
		int j = 0;
		while (m2->m[j].len != 0)
		{
			if ((m1->m[i].dest_start <= m2->m[j].src_start) && (m1->m[i].len + m1->m[i].dest_start > m2->m[j].src_start))
			{
				mergeInterval1BelowInterval2(&m1->m[i], &m2->m[j], mM);
			}
			else if ((m1->m[i].dest_start >= m2->m[j].src_start) && (m1->m[i].dest_start < m2->m[j].src_start + m2->m[j].len))
			{
				mergeInterval1UpperInterval2(&m1->m[i], &m2->m[j], mM);
			}
			else
			{//no overlapping just copy
				mM->m[mM->nb_ranges].src_start = m2->m[j].src_start;
				mM->m[mM->nb_ranges].dest_start = m2->m[j].dest_start;
				mM->m[mM->nb_ranges].len = m2->m[j].len;
				mM->nb_ranges++;
			}
			j++;
		}
		i++;
	}
}

static void mergeInterval1BelowInterval2(struct Map_line* m1, struct Map_line* m2, struct Map* merged)
{// m1.dest_start < m2.src_start && m1.len + m1.dest_start > m2.srcstart
	int* offset = &(merged->nb_ranges);
	merged->m[*offset].src_start = m1->src_start;
	merged->m[*offset].len = (m2->src_start - m1->dest_start);
	merged->m[*offset].dest_start = m1->dest_start;
	(*offset)++;
	if ( (m1->dest_start + m1->len) <= (m2->src_start + m2->len) ) //cas 1 ou 2
	{
		merged->m[*offset].src_start = m1->src_start + (m2->src_start - m1->dest_start);
		merged->m[*offset].len = m1->len - (m2->src_start - m1->dest_start);
		merged->m[*offset].dest_start = m2->dest_start;
		(*offset)++;
		if ((m1->dest_start + m1->len) != (m2->src_start + m2->len))
		{
			merged->m[*offset].src_start = m2->src_start + merged->m[(*offset) - 1].len;
			merged->m[*offset].len = m2->len - merged->m[(*offset)-1].len;
			merged->m[*offset].dest_start = m2->dest_start + merged->m[(*offset) - 1].len;
			(*offset)++;
		}
	}
	else //cas 3 
	{
		merged->m[*offset].src_start = m1->src_start + (m2->src_start - m1->dest_start);
		merged->m[*offset].len = m2->len - (m2->src_start - m1->dest_start);
		merged->m[*offset].dest_start = m2->dest_start;
		(*offset)++;
		merged->m[*offset].src_start = m1->src_start + (m2->len + (m2->src_start - m1->dest_start));
		merged->m[*offset].len = m1->len - (m2->len + (m2->src_start - m1->dest_start));
		merged->m[*offset].dest_start = m1->dest_start + (m2->len + (m2->src_start - m1->dest_start));
		(*offset)++;
	}
}

static void mergeInterval1UpperInterval2(struct Map_line* m1, struct Map_line* m2, struct Map* merged)
{// m1.dest_start > m2.src_start && m1.dest_start < m2.srcstart + m2.len
	int* offset = &(merged->nb_ranges);
	merged->m[*offset].src_start = m2->src_start;
	merged->m[*offset].len = (m1->dest_start - m2->src_start);
	merged->m[*offset].dest_start = m2->dest_start;
	(*offset)++;
	if ((m1->dest_start + m1->len) <= (m2->src_start + m2->len)) //cas 1 ou 2
	{
		merged->m[*offset].src_start = m1->src_start + (m1->dest_start - m2->src_start);
		merged->m[*offset].len = m1->len;
		merged->m[*offset].dest_start = m2->dest_start + (m2->src_start - m1->dest_start);
		(*offset)++;
		if ((m1->dest_start + m1->len) != (m2->src_start + m2->len)) //cas 1
		{
			merged->m[*offset].src_start = m2->src_start + m1->len + (m1->dest_start - m2->src_start);
			merged->m[*offset].len = m2->len - (m1->len + (m1->dest_start - m2->src_start));
			merged->m[*offset].dest_start = m2->dest_start + (m1->len + (m1->dest_start - m2->src_start));
			(*offset)++;
		}
	}
	else //cas 3 
	{
		merged->m[*offset].src_start = m1->src_start + (m1->dest_start - m2->src_start);
		merged->m[*offset].len = m2->len;
		merged->m[*offset].dest_start = m2->dest_start + (m2->src_start - m1->dest_start);
		(*offset)++;
		merged->m[*offset].src_start = m1->src_start + (m2->len + (m2->src_start - m1->dest_start));
		merged->m[*offset].len = m1->len - (m2->len + (m2->src_start - m1->dest_start));
		merged->m[*offset].dest_start = m1->dest_start + (m2->len + (m2->src_start - m1->dest_start));
		(*offset)++;
	}
}
*/