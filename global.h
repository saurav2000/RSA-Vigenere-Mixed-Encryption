#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <fstream>
#include <string>
#include <gmpxx.h>

#define big_int mpz_class

int char_to_int(char x)
{
	switch(x)
	{
		case ' ':
		return 26;
		case '.':
		return 27;
		case ',':
		return 28;
		case '\'':
		return 29;
	}
	if(x<=122 && x>=97)
		return x-97;
	if(x<=57 && x>=48)
		return x-18;
	return -1;
}

char int_to_char(int x)
{
	int res = 0;
	switch(x)
	{
		case 26:
		return ' ';
		case 27:
		return '.';
		case 28:
		return ',';
		case 29:
		return '\'';
	}
	if(x>=0 && x<=25)
		return x+97;
	if(x>=30 && x<=39)
		return x+18;
	return 0;
}

#endif