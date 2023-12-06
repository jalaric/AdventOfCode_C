#include "stdio.h"
#include "string.h"
#include "stdbool.h"

void GetLine(char* buffer, char* dest, size_t size);
int GetNumbersInLine(char* line, int* array);
int IsNumber(char c);