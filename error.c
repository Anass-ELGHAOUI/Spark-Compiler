#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

#define debug true

error* errorsList = NULL;

error* addOnTabError(error* element){
	error* new = (error*)malloc(sizeof(error));
	new->line = element->line;
	strcpy(new->msgError, element->msgError);
	new->next = NULL;
	if(errorsList == NULL)
	{
		errorsList = new;
	}else{
		error* tmp = errorsList;
		while(tmp->next != NULL){ 
			tmp = tmp->next;
		}
		tmp->next = new;
	}
	return errorsList;
}

void showErrors(){
	error* tmp = errorsList;
	int i=1;
	while(tmp != NULL){
		printf("error %d in line %d : %s\n",i++,tmp->line,tmp->msgError); 
		tmp = tmp->next;
	}
}

