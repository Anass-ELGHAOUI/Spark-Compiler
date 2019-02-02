

typedef enum{
	ADD,SUB,MUL,DIV,EQL,NE,GTR,LSS,GEQ,LEQ,PRN,INN,INT,LDI,LDA,LDV,STO,BZN,BZE,HLT
}Pcode;

typedef struct{
	Pcode inst;
	int num;
}instruction;



//function de generation de code

void genererMiInst(Pcode code);
void genererInst(Pcode code,int num);
char* diplayOnCode(Pcode code);
void displayAllPcode();



