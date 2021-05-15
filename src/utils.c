#include <string.h>

extern int validateName(char name[]) {
	if(strlen(name) > 0)
		return 0;
	return 1;
}