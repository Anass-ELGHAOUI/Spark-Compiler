#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "analyseur_spark.h"
#include "generateurCode.h"
#define debug true
#define tailleCode 200



//tableau de code
instruction tabCode[tailleCode];

//IndexPile
int ip=-1; //Pour rester pointer sur la derinere inst inserer

void genererMiInst(Pcode code){

	if(ip<tailleCode){
		tabCode[++ip].inst=code;
	}else{
		//Affichage d'erreur
	}
	

}

void genererInst(Pcode code,int num){
	
	if(ip<tailleCode){
		tabCode[++ip].inst=code;
		tabCode[ip].num=num;
	}else{
		//Affichage d'erreur
	}

}


char* diplayOnCode(Pcode code){
	switch (code){
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
		printf("\t%s\t%d\n",diplayOnCode(tabCode[i].inst),tabCode[i].num);
	}
}



