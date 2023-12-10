#include "days.h"
#include <time.h>
#include <stdio.h>


int main()
{
	printf("------------------------------------- Benchmark AOC in C -------------------------------------\n");
	printf("CPU used : i7-9750H CPU @ 2.60GHz, 2601 Mhz, 6 Core(s), 12 Logical Processor(s)\n");
	printf("----------------------------------------------------------------------------------------------\n");
	for (int i = 0; i < kAocDays; i++)
	{
		if (i != 4) // day 5 is not optimized yet
		{
			initDay(i);
			clock_t time = clock();
			if (runDay(i, false))//part1
			{
				time = clock() - time;
				printf("\n ------------------------ Day %d ------------------------ \n", i + 1);
				outputResult(i);
				printf("Day %2d part 1: %f s\n", i + 1, (((float)time) / CLOCKS_PER_SEC));
				time = clock();
				runDay(i, true);//part2
				time = clock() - time;
				outputResult(i);
				printf("Day %2d part 2: %f s\n", i + 1, (((float)time) / CLOCKS_PER_SEC));
				printf("\n --------------------------------------------------- \n");
			}
		}
	}
	
	return 0;
}
