#include "days.h"
#include "../common_lib.h"
#pragma warning(disable : 4996)


#define MAX_LINES (2000)
#define MAX_LINE_LEN (250)

static int getRank(char* buf, __int64* sum);
static int getRankPart2(char* buf, __int64* sum);
void bubbleSort(char arr[], int n);
void bubbleSortCards(int* rank, int* bids, __int64* stren, int n);
static int getBid(char* buf);
void swap_int(int* xp, int* yp);
void swap_int64(__int64* xp, __int64* yp);
static void TransformLetter(char* buf, bool part);

static __int64 result = 0;

void InitDay7() {
}
void ResultDay7() {
	printf("Result : %lld\n", result);
}

static int ranks[MAX_LINES] = { 0 };
static int bids[MAX_LINES] = { 0 };
static __int64 strength[MAX_LINES] = { 0 };

int day7(bool part)
{
	FILE* fp;
	fp = fopen("day7.data", "r");
	char line[MAX_LINE_LEN] = "";
	result = 0;
	int hands = 0;
	while (!feof(fp))
	{
		fgets(line, MAX_LINE_LEN, fp);
		if(part == true)
			ranks[hands] = getRankPart2(line, &strength[hands], part);
		else
			ranks[hands] = getRank(line, &strength[hands], part);
		bids[hands] = getBid(&line[6]);
		hands++;
	}
	fclose(fp);
	bubbleSortCards(ranks, bids, strength, hands);
	for (__int64 j = 0; j < hands; j++)
	{
		result += (j + 1) * bids[j];
	}
	return 0;
}

static void TransformLetter(char* buf, bool part)
{
	for (int i = 0; i < 5; i++)
	{
		if (part == true && buf[i] == 'J')
		{
			buf[i] = '1';
		}
		else
		{
			char o = 0x40;
			switch (buf[i])
			{
			case 'A': o++;
			case 'K': o++;
			case 'Q': o++;
			case 'J': o++;
			case 'T':
				buf[i] = o;
				break;
			default:
				break;
			}
		}
	}
}

static int getBid(char* buf)
{
	int ret = 0;
	int i = 0;
	int n = IsNumber(buf[0]);
	while (n != -1)
	{
		i++;
		ret *= 10;
		ret += n;
		n = IsNumber(buf[i]);
	}
	return ret;
}

static int getRank(char* buf, __int64* sum)
{
	char c1 = 0;
	char c2 = 0;
	char t = 0;
	char cards[5];
	memcpy(cards, buf, 5);
	TransformLetter(cards, false);
	*sum = (__int64)( ((__int64)cards[0]) << 32) + (((__int64)cards[1]) << 24) + (((__int64)cards[2]) << 16)
	+ (((__int64)cards[3]) << 8) + ((__int64)cards[4]);
	bubbleSort(cards, 5);
	//get rank
	for (int j = 1; j < 5; j++)
	{
		if (cards[j] == cards[j - 1])
		{
			if ((c1 == 0) || (t == cards[j]))
			{
				c1++;
				t = cards[j];
			}
			else
			{
				c2++;
			}
		}
	}
	if (c1 > c2)
		return c1 * 10 + c2;
	return c2 * 10 + c1;
}

static int getRankPart2(char* buf, __int64* sum)
{
	char c1 = 0;
	char c2 = 0;
	char joker = 0;
	char t = 0;
	char cards[5];
	memcpy(cards, buf, 5);
	TransformLetter(cards, true);
	*sum = (__int64)(((__int64)cards[0]) << 32) + (((__int64)cards[1]) << 24) + (((__int64)cards[2]) << 16)
		+ (((__int64)cards[3]) << 8) + ((__int64)cards[4]);
	bubbleSort(cards, 5);
	//get rank
	if (cards[0] == '1')
		joker++;
	for (int j = 1; j < 5; j++)
	{
		if (cards[j] != '1')
		{
			if (cards[j] == cards[j - 1])
			{
				if ((c1 == 0) || (t == cards[j]))
				{
					c1++;
					t = cards[j];
				}
				else
				{
					c2++;
				}
			}
		}
		else
		{
			joker++;
		}
	}
	if (joker == 5)
		joker--;
	if (c1 > c2) {
		c1 += joker;
		return c1 * 10 + c2;
	}
	c2 += joker;
	return c2 * 10 + c1;
}

void swap(char* xp, char* yp)
{
	char temp = *xp;
	*xp = *yp;
	*yp = temp;
}

// An optimized version of Bubble Sort
void bubbleSort(char arr[], int n)
{
	int i, j;
	bool swapped;
	for (i = 0; i < n - 1; i++) {
		swapped = false;
		for (j = 0; j < n - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(&arr[j], &arr[j + 1]);
				swapped = true;
			}
		}

		// If no two elements were swapped by inner loop,
		// then break
		if (swapped == false)
			break;
	}
}

// An optimized version of Bubble Sort
void bubbleSortCards(int* rank, int* bids, __int64* stren, int n)
{
	int i, j;
	bool swapped;
	for (i = 0; i < n - 1; i++) {
		swapped = false;
		for (j = 0; j < n - i - 1; j++) {
			if (rank[j] > rank[j + 1]) {
				swap_int(&rank[j], &rank[j + 1]);
				swap_int(&bids[j], &bids[j + 1]);
				swap_int64(&stren[j], &stren[j + 1]);
				swapped = true;
			}
			else if (rank[j] == rank[j + 1])
			{
				if (stren[j] > stren[j + 1])
				{
					swap_int(&rank[j], &rank[j + 1]);
					swap_int(&bids[j], &bids[j + 1]);
					swap_int64(&stren[j], &stren[j + 1]);
					swapped = true;
				}
			}
		}
		// If no two elements were swapped by inner loop,
		// then break
		if (swapped == false)
			break;
	}
}

void swap_int(int* xp, int* yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void swap_int64(__int64* xp, __int64* yp)
{
	__int64 temp = *xp;
	*xp = *yp;
	*yp = temp;
}