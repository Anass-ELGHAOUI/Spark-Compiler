

typedef enum{
	ADD,SUB,ANDN,AD,XR,MUL,DIV,EQL,NE,GTR,LSS,GEQ,LEQ,PRN,INN,INT,LDI,LDA,LDV,STO,BZN,BZE,HLT
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




//function de generation de code

void genererMiInst(Pcode code);
void genererInstInt(Pcode code,int num);
void genererInstFloat(Pcode code,float decimal);
void genererInstString(Pcode code,char* string);
char* diplayOnCode(Pcode code);
void displayAllPcode();



