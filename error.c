#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "error.h"

#define debug true

error* errorsList = NULL;

static int NBERRORS = 0;
// adding a new error to the errors list
void addOnTabError(error* element){
	if(debug) printf("_in_add_On_Tab_Error_\n");
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
	NBERRORS++;
	if(debug) printf("_out_add_On_Tab_Error_\n");
}
//show all the errors
void showErrors(){
	if(debug) printf("_in_show_Errors_\n");
	error* tmp = errorsList;
	int i=1;
	while(tmp != NULL){
		printf("error %d in line %d : %s\n",i++,tmp->line,tmp->msgError); 
		tmp = tmp->next;
	}
	if(debug) printf("_out_show_Errors_\n");
}
//geting the number of errors detected
int getNbrErrors(){
	return NBERRORS;
}

