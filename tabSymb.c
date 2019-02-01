#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "analyseur_spark.h"
#include "tabSymb.h"
#define debug true

#define NBS 100

static tabSymb TS[NBS];

static valueSymb VALUE[NBS];

static int NBVAR = 0;

bool addOnTabSymb(tabSymb element){
	if(debug) printf("_in_addOntabSymb_function_\n");
	bool result = false;
	if(NBVAR < NBS){
		TS[NBVAR].type = element.type;
		TS[NBVAR].adresse = element.adresse;
		strcpy(TS[NBVAR].name, element.name);
		NBVAR++;
		result = true;
	}
	if(debug) printf("_out_addOntabSymb_function_\n");
	return result;
}
bool isInTabSymb(char* nom){
	if(debug) printf("_in_isOntabSymb_function_\n");
	bool result = false;
	int i=0;
	for(i=0;i<NBVAR;++i){
		if(strcmp(TS[i].name,nom)==0)
			result = true;	
	}
	if(debug) printf("_out_isOntabSymb_function_\n");
	return result;
}
void showAllSymbols(){
	if(debug) printf("_in_showAllSymb_function_\n");
	int i=0;
	for(int i=0;i<NBVAR;i++){
		printf(" symbole %d -> name : %s , adresse : %d ;\n",i+1,TS[i].name,TS[i].adresse); 
	}
	if(debug) printf("_out_showAllSymb_function_\n");	

}
