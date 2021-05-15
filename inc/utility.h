#ifndef UTILITY_H_
#define UTILITY_H_

typedef enum PhonebookStrErr
{
	STR_OK,
	LEN_EQUAL_ZERO,
	FIRST_IS_SPACE,
	END_IS_SPACE,
	NUMBER_10,
	NUMBER_FIRST_0
}PhonebookStrErr;

extern int validate_name(char name[]);

extern int validate_number(char number[]);

#endif