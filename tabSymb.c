#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "analyseur_spark.h"
#include "tabSymb.h"
#define debug true

#define NBS 100

static tabSymb TS[NBS];

static int NBVAR = 0;

void addOnTabSymb(tabSymb element){
	if(NBVAR < NBS){
		TS[NBVAR].type = element.type;
		TS[NBVAR].adresse = element.adresse;
		strcpy(TS[NBVAR].name, element.name);
		NBVAR++;
	}
}
bool isInTabSymb(char* nom){
	int i=0;
	for(i=0;i<NBVAR;++i){
		if(strcmp(TS[i].name,nom)==0)
			return true;	
	}
	return false;
}
void showAllSymbols(){
	int i=0;
	for(int i=0;i<NBVAR;i++){
		printf(" symbole %d -> name : %s , adresse : %d ;\n",i+1,TS[i].name,TS[i].adresse); 
	}	

}
