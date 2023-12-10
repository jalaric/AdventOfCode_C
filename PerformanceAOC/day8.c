#include "days.h"
#include "../common_lib.h"
#include <stdbool.h>
#pragma warning(disable : 4996)


#define MAX_LINES (2000)
#define MAX_LINE_LEN (300)

static __int64 result = 0;

typedef struct intersection {
	int name_hash;
	int right_hash;
	int left_hash;
	struct intersection* pRight;
	struct intersection* pLeft;
} intersec;

void InitDay8() {
}
void ResultDay8() {
	printf("Result : %lld\n", result);
}

static int SetSteps(bool* steps, char* line);
static void FillHash(char* line, intersec* inter);
static void FindLeftRightHash(intersec* arr, int len);
static __int64 findlcm(__int64 arr[], int n);


static bool steps[MAX_LINE_LEN] = { false };
static intersec x[MAX_LINES] = { 0 };

int day8(bool part)
{
	intersec* pStart = NULL;
	intersec* pStartP2[20];
	int P2_starts = 0;
	const int startHash = 0x414141; //AAA
	FILE* fp;
	fp = fopen("day8.data", "r");
	char line[MAX_LINE_LEN] = "";
	//get steps
	fgets(line, MAX_LINE_LEN, fp);
	int len = SetSteps(steps, line);
	int i = 0;
	fgets(line, MAX_LINE_LEN, fp);//empty
	while (!feof(fp))
	{
		fgets(line, MAX_LINE_LEN, fp);
		FillHash(line, &x[i]);
		if ((x[i].name_hash == startHash) && (part == false)) //part 1
			pStart = &x[i];
		if ((part == true) && ( (x[i].name_hash & 0xFF) == 0x41))//part2
		{
			pStartP2[P2_starts] = &x[i];
			P2_starts++;
		}
		i++;
	}
	fclose(fp);
	FindLeftRightHash(x, i);
	if (part == false)
	{//process steps
		int s = 0;
		result = 0;
		int end_hash = (((int)'Z') << 16) + (((int)'Z') << 8) + (int)'Z';
		while (pStart->name_hash != end_hash)
		{
			if (steps[s] == false) //left
			{
				pStart = pStart->pLeft;
			}
			else//right
			{
				pStart = pStart->pRight;
			}
			s = (++s) % len;
			result++;
		}
	}
	else //part2
	{//process steps
		__int64 result_a[20] = { 0 };
		result = 0;
		for (int w = 0; w < P2_starts; w++)
		{
			int s = 0;
			result_a[w] = 0;
			while ((char)(pStartP2[w]->name_hash & 0xFF) != 'Z') {
				if (steps[s] == false) //left
					pStartP2[w] = pStartP2[w]->pLeft;
				else
					pStartP2[w] = pStartP2[w]->pRight;
				result_a[w]++;
				s = (result_a[w] % len);
			}
			//result_a[w]--;
			s = 0;
		}
		result = findlcm(result_a, P2_starts);
	}
	return 0;
}

// Utility function to find
// GCD of 'a' and 'b'
int gcd(__int64 a, __int64 b)
{
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

// Returns LCM of array elements
static __int64 findlcm(__int64 arr[], int n)
{
	__int64 ans = arr[0];
	for (int i = 1; i < n; i++)
		ans = (arr[i]*ans)  / gcd(arr[i], ans);
	return ans;
}

static void FillHash(char* line, intersec* inter)
{
	inter->name_hash = (((int) line[0]) << 16) + (((int)line[1]) << 8) + (int)line[2];
	inter->left_hash = (((int)line[7]) << 16) + (((int)line[8]) << 8) + (int)line[9];
	inter->right_hash = (((int)line[12]) << 16) + (((int)line[13]) << 8) + (int)line[14];
}

static void FindLeftRightHash(intersec* arr, int len)
{
	for(int i = 0; i < len; i++)
	{
		for (int j = 0; j < len; j++)
		{
			if (arr[i].left_hash == arr[j].name_hash)
			{
				arr[i].pLeft = &arr[j];
			}
			if (arr[i].right_hash == arr[j].name_hash)
			{
				arr[i].pRight = &arr[j];
			}
		}
	}
}

static int SetSteps(bool* steps, char* line)
{
	int i = 0;
	while (line[i] != '\n')
	{
		if (line[i] == 'R')
			steps[i] = true;
		else
			steps[i] = false;
		i++;
	}
	return i;
}