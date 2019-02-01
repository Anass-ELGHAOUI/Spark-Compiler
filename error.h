
typedef struct error{
	char msgError[200];
	int line;
	struct error* next;
}error;


extern error* addOnTabError(error* element);
extern void showErrors();
