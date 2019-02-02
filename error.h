
typedef struct error{
	char msgError[200];
	int line;
	struct error* next;
}error;


extern void addOnTabError(error* element);
extern void showErrors();
extern int getNbrErrors();
