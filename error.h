#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct error{
	char msgError[200];
	int line;
	struct error* next;
}error;


extern error* addOnTabError(error* element);
extern void showErrors();
