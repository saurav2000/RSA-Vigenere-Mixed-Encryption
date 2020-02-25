#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <gmpxx.h>

using namespace std;

#define big_int mpz_class

map<char, int> char_to_int;
map<int, char> int_to_char;

void map_init()
{
	for(int i=97;i<123;++i)
	{
		char ch = (char)i;
		char_to_int[ch] = i-97;
		int_to_char[i-97] = ch;
	}
	//space
	char_to_int[' '] = 26;
	int_to_char[26] = ' ';
	//fullstop
	char_to_int['.'] = 27;
	int_to_char[27] = '.';
	//comma
	char_to_int[','] = 28;
	int_to_char[28] = ',';
	//apostrophe
	char_to_int['\''] = 29;
	int_to_char[29] = '\'';

	for(int i=48;i<58;++i)
	{
		char ch = (char)i;
		char_to_int[ch] = i-18;
		int_to_char[i-18] = ch;	
	}
}

big_int ext_gcd(big_int a, big_int b, big_int *x, big_int *y)
{
	if(a==0)
	{
		*x = 0;
		*y = 1;
		return b;
	}
	big_int x1, y1;
	big_int gcd = ext_gcd(b%a, a, &x1, &y1);
	*x = y1 - (b/a) * x1;
	*y = x1;
	return gcd;
}

big_int mod_inv(big_int a, big_int n)
{
	big_int x, y;
	ext_gcd(a, n, &x, &y);
	return ((x%n)+n)%n;
}

big_int mod_exp(big_int a, big_int b, big_int n)
{
	big_int res = 1;
	while(b!=0)
	{
		if((b&1)==1)
			res = (res * a) % n;
		b = b>>1;
		a = (a*a) % n;
	}
	return res;
}

bool miller_rabin(gmp_randclass &rand_gen, big_int n)
{
	if(n==0 || n==1 || ((n&1)==0))
		return false;
	big_int s = 0, r = n-1;
	while((r&1)!=1)
	{
		++s;
		r = r>>1;
	}

	for(int cnt=0;cnt<20;++cnt)
	{
		big_int a = 0;
		while(a!=1 && gcd(a,n)!=1)
			a = rand_gen.get_z_range(n);
		big_int x = mod_exp(a,r,n);
		int i;
		for(i=0;i<s;++i)
		{
			if((i==0 && x==1) || x==n-1)
				break;
			if(i>0 && x==1)
				return false;
			x = (x*x)%n;
		}
		if(i==s)
			return false;
	}
	return true;
}

big_int generate_prime(gmp_randclass &rand_gen, big_int bit_cnt)
{
	big_int a = 0;
	while(!miller_rabin(rand_gen, a))
	{
		a = 0;
		while((a&1)!=1)
			a = rand_gen.get_z_bits(bit_cnt);
	}
	return a;
}

big_int generate_strong_prime(gmp_randclass &rand_gen, big_int bit_cnt)
{
	big_int s = generate_prime(rand_gen, bit_cnt), t = generate_prime(rand_gen, bit_cnt), p = 0, r = 0;
	for(big_int i = rand_gen.get_z_bits(5);;++i)
	{
		r = 2*i*t + 1;
		if(miller_rabin(rand_gen, r))
			break;
	}

	big_int p0 = 2*mod_exp(s,r-2,r)*s - 1;
	for(big_int j = rand_gen.get_z_bits(5);;++j)
	{
		p = p0 + 2*j*r*s;
		if(miller_rabin(rand_gen, p))
			break;
	}	

	return p;
}

void generate_key(gmp_randclass &rand_gen, big_int bit_cnt, char *public_file, char *private_file)
{
	big_int p = generate_strong_prime(rand_gen, bit_cnt), q = generate_strong_prime(rand_gen, bit_cnt);
	big_int N = p*q;
	big_int phi = (p-1)*(q-1);
	big_int e = 0;
	while(e!=1 && gcd(e, phi)!=1)
		e = rand_gen.get_z_range(phi);
	big_int d = mod_inv(e, phi);
	ofstream fout(public_file);
	fout<<e<<"\n"<<N<<"\n";
	fout.close();
	fout.open(private_file);
	fout<<d<<"\n"<<e<<"\n"<<N<<"\n"<<p<<"\n"<<q<<"\n";
	fout.close();
}

string RSA_encrypt(string input, big_int e, big_int N)
{
	big_int M = 0, mult = 1;
	for(int i=0;i<input.length();++i)
	{
		M+= (char_to_int[input[i]] * mult);
		mult = mult*40;
	}

	big_int C = mod_exp(M, e, N);
	string encrypted = "";
	while(C!=0)
	{
		big_int x = C%40;
		encrypted+= int_to_char[x.get_si()];
		C = C/40;
	}

	return encrypted;
}

string RSA_decrypt(string input, big_int d, big_int N, big_int p, big_int q)
{
	big_int C = 0, mult = 1;
	for(int i=0;i<input.length();++i)
	{
		C+= (char_to_int[input[i]] * mult);
		mult = mult*40;
	}
	big_int M = mod_exp(C, d, N);
	string decrypted = "";
	while(M!=0)
	{
		big_int x = M%40;
		decrypted+= int_to_char[x.get_si()];
		M = M/40;
	}

	return decrypted;
}

void RSA_encrypt(char *key_file, char *text_file, char *output_file)
{
	ifstream fin;
	fin.open(key_file);
	big_int e, N;
	fin>>e>>N;
	fin.close();

	fin.open(text_file);
	string res = "", line ;
	while(getline(fin, line))
		res+= (line+" ");
	fin.close();


	ofstream fout(output_file);
	// fout<<encrypted<<"\n";
	fout.close();
}

string vigenere(string a, string key, bool encrypt)
{
	string res = "";
	int n = key.length();
	for(int i=0;i<a.length();++i)
	{
		if(encrypt)
			res+= int_to_char[((char_to_int[a[i]]+char_to_int[key[i%n]])%40)];
		else
			res+= int_to_char[((char_to_int[a[i]]-char_to_int[key[i%n]]+40)%40)];
	}
	return res;
}



int main(int argc, char const *argv[])
{
	gmp_randclass rand_gen(gmp_randinit_mt);
	rand_gen.seed(time(NULL));
	map_init();


	return 0;
}