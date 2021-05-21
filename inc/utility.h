#ifndef UTILITY_H_
#define UTILITY_H_

enum PhoneBookErr
{
	STR_OK,
	LEN_EQUAL_ZERO,
	FIRST_IS_SPACE,
	END_IS_SPACE,
	NUMBER_10,
	NUMBER_FIRST_ERR	//K có + hoặc 0 ở đầu sđt
};

extern int validate_name(char name[]);

extern int validate_number(char number[]);

#endif