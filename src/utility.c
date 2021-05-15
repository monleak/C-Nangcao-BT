#include <string.h>

#include "../inc/utility.h"

extern int validate_name(char name[]) {
	int len = strlen(name);
	if(len == 0) {
		return LEN_EQUAL_ZERO;
	}else if(name[0] == ' ') {
		return FIRST_IS_SPACE;
	}else if(name[len-1] == ' ') {
		return END_IS_SPACE;
	}
	return STR_OK;
}

extern int validate_number(char number[]) {
	int len = strlen(number);
	if(len == 0) {
		return LEN_EQUAL_ZERO;
	}else if (len < 10) {
		return NUMBER_10;
	}
	return STR_OK;
}