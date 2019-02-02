
typedef struct tabSymb{
	char name[30];
	typetoken type;
	int adresse;
	bool isCste;
}tabSymb;


typedef union valueSymb{
	char stringValue[100];
	float floatValue;
	char charValue;
	int intValue;
}valueSymb;

typedef struct{
	char* value;
}idfvaluetype;

typedef struct{
	char* value;
}stringvaluetype;

typedef struct{
	int line;
}linevalue;

typedef struct{
	int value;
}numericvaluetype;

typedef struct{
	float value;
}decimalvaluetype;


extern bool addOnTabSymb(tabSymb element);
extern bool isInTabSymb(char* nom);
extern int adresseInTabSymb(char* nom);
extern void showAllSymbols();
extern int getNbrSymb();
extern tabSymb getSymbByName(char *nom);
extern void addIntValueOfSymb(int adresse,int value);
extern void addFloatValueOfSymb(int adresse,float value);
extern void addStringValueOfSymb(int adresse,char* value);
