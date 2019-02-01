
typedef struct tabSymb{
	char name[30];
	typetoken type;
	int adresse;
}tabSymb;


typedef union valueSymb{
	char StringValue[100];
	float floatValue;
	char charValue;
	int intValue;
}valueSymb;


extern bool addOnTabSymb(tabSymb element);
extern bool isInTabSymb(char* nom);
extern void showAllSymbols();
