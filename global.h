#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <gmpxx.h>
int char_to_int(char x);
char int_to_char(int x);
mpz_class mod_exp(mpz_class a, mpz_class b, mpz_class n);

#endif

#ifndef big_int
#define big_int mpz_class
#endif
