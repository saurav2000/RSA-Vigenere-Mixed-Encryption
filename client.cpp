#include "client.h"

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