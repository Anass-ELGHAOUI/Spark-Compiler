#define tailleCode 200


typedef enum{
	ADD,SUB,ANDN,AD,XR,MUL,DIV,EQL,NE,GTR,LSS,GEQ,LEQ,PRN,INN,INT,LDI,LDA,LDV,STO,BRN,BZE,HLT
}Pcode;


typedef union paramInst{
	char stringValue[100];
	float floatValue;
	char charValue;
	int intValue;
}paramInst;

typedef struct{
	Pcode inst;
	typetoken type;
	paramInst paramI;
}instruction;


//tableau de code
instruction tabCode[tailleCode];

//function de generation de code

extern void genererMiInst(Pcode code);
extern void genererInstInt(Pcode code,int num);
extern void genererInstFloat(Pcode code,float decimal);
extern void genererInstString(Pcode code,char* string);
extern char* diplayOnCode(Pcode code);
extern void displayAllPcode();
extern int getCurrentIndexPile();
