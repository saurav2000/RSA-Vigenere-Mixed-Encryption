#include "global.h"

int char_to_int(char x)
{
	switch(x)
	{
		case ' ':
		return 0;
		case '.':
		return 1;
		case ',':
		return 2;
		case '\'':
		return 3;
	}
	if(x<=122 && x>=97)
		return x-93;
	if(x<=57 && x>=48)
		return x-18;
	return -1;
}

char int_to_char(int x)
{
	int res = 0;
	switch(x)
	{
		case 0:
		return ' ';
		case 1:
		return '.';
		case 2:
		return ',';
		case 3:
		return '\'';
	}
	if(x>=4 && x<=29)
		return x+93;
	if(x>=30 && x<=39)
		return x+18;
	return 0;
}

mpz_class mod_exp(mpz_class a, mpz_class b, mpz_class n)
{
	mpz_class res = 1;
	while(b!=0)
	{
		if((b&1)==1)
			res = (res * a) % n;
		b = b>>1;
		a = (a*a) % n;
	}
	return res;
}
