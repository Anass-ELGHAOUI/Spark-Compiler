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
			case AND:return "AND";
			case XOR:return "XOR";
			case ANDN:return "ANDN";
			case HLT:return "HLT";
			case BZE:return "BZE"; 
			case BRN:return "BRN"; 
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
	FILE* fp;
	fp = fopen("pseudoCode.txt","w");
	for(i=0;i<ip+1;i++){
		printf("%d\t%s",i,diplayOnCode(tabCode[i].inst));
		fprintf(fp,"%d\t%s",i,diplayOnCode(tabCode[i].inst));

		switch(tabCode[i].type){
			case INTEGER : printf("\t%d",tabCode[i].paramI.intValue); fprintf(fp,"\t%d",tabCode[i].paramI.intValue); break;	
			case FLOAT : printf("\t%f",tabCode[i].paramI.floatValue); fprintf(fp,"\t%f",tabCode[i].paramI.floatValue); break;
			case STRING : printf("\t%s",tabCode[i].paramI.stringValue); fprintf(fp,"\t%s",tabCode[i].paramI.stringValue);break;	
		} 
		printf("\n");
		fprintf(fp,"\n");
	}
	fclose(fp);
}

int getCurrentIndexPile(){
	return ip;
}



