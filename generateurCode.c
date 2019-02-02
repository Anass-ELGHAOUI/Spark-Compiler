#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "analyseur_spark.h"
#include "generateurCode.h"
#define debug true


//IndexPile
int ip=-1; //Pour rester pointer sur la derinere inst inserer

void genererMiInst(Pcode code){

	if(ip<tailleCode){
		tabCode[++ip].inst=code;
	}else{
		//Affichage d'erreur
	}
	

}

void genererInstInt(Pcode code,int num){
	
	if(ip<tailleCode){
		tabCode[++ip].inst=code;
		tabCode[ip].type=INTEGER;
		tabCode[ip].paramI.intValue=num;
	}else{
		//Affichage d'erreur
	}

}

void genererInstFloat(Pcode code,float decimal){
	if(ip<tailleCode){
		tabCode[++ip].inst=code;
		tabCode[ip].type=FLOAT;
		tabCode[ip].paramI.floatValue=decimal;
	}else{
		//Affichage d'erreur
	}
}

void genererInstString(Pcode code,char* string){
	if(ip<tailleCode){
		tabCode[++ip].inst=code;
		tabCode[ip].type=STRING;
		strcpy(tabCode[ip].paramI.stringValue,string);
	}else{
		//Affichage d'erreur
	}
}


char* diplayOnCode(Pcode code){
	switch (code){	
			case AND:return "AD";
			case XOR:return "XR";
			case ANDN:return "ANDN";
			case HLT:return "HLT";
			case BZE:return "BZE";
			case BZN:return "BZN";
			case INT:return "INT";
			case LEQ:return "LEQ";
			case GEQ:return "GEQ";
			case LSS:return "LSS";
			case GTR:return "GTR";
			case NE:return "NE";
			case EQL:return "EQL";
			case LDA:return "LDA";
			case STO:return "STO";
			case LDI:return "LDI";
			case ADD:return "ADD";
			case SUB:return "SUB";
			case MUL:return "MUL";
			case DIV:return "DIV";
			case LDV:return "LDV";
			case PRN:return "PRN";
			case INN:return "INN";
	}
}

void displayAllPcode(){
	int i;
	for(i=0;i<ip+1;i++){
		printf("\t%s",diplayOnCode(tabCode[i].inst));

		switch(tabCode[i].type){
			case INTEGER : printf("\t%d",tabCode[i].paramI.intValue); break;	
			case FLOAT : printf("\t%f",tabCode[i].paramI.floatValue); break;
			case STRING : printf("\t%s",tabCode[i].paramI.stringValue); break;	
		} 
		printf("\n");
	}
}

int getCurrentIndexPile(){
	return ip;
}



