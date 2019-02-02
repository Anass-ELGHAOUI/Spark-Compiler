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

//adding a new symbole to the tab
bool addOnTabSymb(tabSymb element){
	if(debug) printf("_in_addOntabSymb_function_\n");
	bool result = false;
	if(NBVAR < NBS){
		TS[NBVAR].type = element.type;
		TS[NBVAR].adresse = element.adresse;
		TS[NBVAR].isCste = element.isCste;
		strcpy(TS[NBVAR].name, element.name);
		NBVAR++;
		result = true;
	}
	if(debug) printf("_out_addOntabSymb_function_\n");
	return result;
}
// test if a symbole is already exist in the tab
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
// show all the symboles
void showAllSymbols(){
	if(debug) printf("_in_showAllSymb_function_\n");
	int i=0;
	for(int i=0;i<NBVAR;i++){
		printf(" symbole %d -> name : %s , adresse : %d ",i+1,TS[i].name,TS[i].adresse);
		switch(TS[i].type){
			case INTEGER : printf(", value : %d\n",VALUE[TS[i].adresse].intValue); break;	
			case FLOAT : printf(", value : %f\n",VALUE[TS[i].adresse].floatValue); break;
			case STRING : printf(", value : %s\n",VALUE[TS[i].adresse].stringValue); break;	
		} 
	}
	if(debug) printf("_out_showAllSymb_function_\n");	

}
// geting the number of variables 
int getNbrSymb(){
	return NBVAR;
}

void addIntValueOfSymb(tabSymb elemen,int value){
	if(debug) printf("_in_addIntValueOfSymb_function_\n");
	VALUE[TS[NBVAR].adresse].intValue = value;
	if(debug) printf("_out_addIntValueOfSymb_function_\n");
}

void addFloatValueOfSymb(tabSymb elemen,float value){
	if(debug) printf("_in_addFloatValueOfSymb_function_\n");
	VALUE[TS[NBVAR].adresse].floatValue = value;
	if(debug) printf("_out_addFloatValueOfSymb_function_\n");
}

void addStringValueOfSymb(tabSymb elemen,char* value){
	if(debug) printf("_in_addStringValueOfSymb_function_\n");
	strcpy(VALUE[TS[NBVAR].adresse].stringValue , value);
	if(debug) printf("_out_addStringValueOfSymb_function_\n");
}

