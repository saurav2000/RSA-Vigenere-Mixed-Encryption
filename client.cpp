#include "client.h"
using namespace std;

int log(big_int n)
{
	int dig = mpz_sizeinbase(n.get_mpz_t(), 40);
	big_int pow;
	mpz_ui_pow_ui(pow.get_mpz_t(), 40, dig-1);
	if(cmp(pow, n)>=0)
		return dig-2;
	else
		return dig-1;
}

string rtrim(const string& s)
{
	size_t end = s.find_last_not_of(" ");
	return (end == string::npos) ? "" : s.substr(0, end + 1);
}

string RSA_encrypt(string input, big_int e, big_int N)
{
	int seg_size = log(N), n = input.length();
	string encrypted = "";

	if(n%seg_size !=0)
	{
		for(int i=0;i<seg_size - (n%seg_size);++i)
			input+= " ";
	}

	for(int j=0;j<input.length();j+=seg_size)
	{
		string s = input.substr(j, seg_size);
		big_int M = 0, mult = 1;
		for(int i=0;i<s.length();++i)
		{
			M+= (char_to_int(s[i]) * mult);
			mult = mult*40;
		}

		big_int C = mod_exp(M, e, N);
		string res = "";
		while(C!=0)
		{
			big_int x = C%40;
			res+= int_to_char(x.get_si());
			C = C/40;
		}

		encrypted+=res;
		for(int i=0;i<seg_size+1-res.length();++i)
			encrypted+=" ";
	}

	return encrypted;
}

string RSA_decrypt(string input, big_int d, big_int N, big_int p, big_int dp, big_int pmul, big_int q, big_int dq, big_int qmul, bool CRT)
{
	int seg_size = log(N)+1, n = input.length();
	string decrypted = "";
	if(n%seg_size!=0)
	{
		for(int i=0;i<seg_size - (n%seg_size);++i)
			input+= " ";
	}

	for(int j=0;j<input.length();j+=seg_size)
	{
		string s = input.substr(j, seg_size);
		big_int C = 0, mult = 1;
		for(int i=0;i<s.length();++i)
		{
			C+= (char_to_int(s[i]) * mult);
			mult = mult*40;
		}

		big_int M = 0;
		if(!CRT)
			M = mod_exp(C, d, N);
		else
		{
			big_int cp = C%p, cq = C%q;
			big_int mp = mod_exp(cp, dp, p), mq = mod_exp(cq, dq, q);
			M = (mp*pmul + mq*qmul)%N;
		}

		string res = "";
		while(M!=0)
		{
			big_int x = M%40;
			res+= int_to_char(x.get_si());
			M = M/40;
		}
		decrypted+=res;
		for(int i=0;i<seg_size-1-(int)res.length();++i)
			decrypted+=" ";
	}

	return decrypted;
}

string vigenere(string a, string key, bool encrypt)
{
	string res = "";
	int n = key.length();
	for(int i=0;i<a.length();++i)
	{
		if(encrypt)
			res+= int_to_char((char_to_int(a[i])+char_to_int(key[i%n]))%40);
		else
			res+= int_to_char((char_to_int(a[i])-char_to_int(key[i%n])+40)%40);
	}
	return res;
}

void encrypt(char *vigenere_key, char *send_priv_key, char *recp_pub_key, char *text_file, char *output_file)
{
	ifstream fin;
	string vig_key;
	big_int d, N_send, p, q, e, N_recp, dp, dq, pmul, qmul;
	
	fin.open(vigenere_key);
	fin>>vig_key;
	fin.close();

	fin.open(send_priv_key);
	fin>>d>>N_send>>p>>dp>>pmul>>q>>dq>>qmul;
	fin.close();

	fin.open(recp_pub_key);
	fin>>e>>N_recp;
	fin.close();

	fin.open(text_file);
	string res = "", line ;
	while(getline(fin, line))
		res+= (line+" ");
	transform(res.begin(), res.end(), res.begin(), ::tolower);
	fin.close();

	res = vigenere(res, vig_key, true);
	string x = res+" "+vig_key;
	x = RSA_encrypt(x, d, N_send);
	x = RSA_encrypt(x, e, N_recp);

	ofstream fout(output_file);
	fout<<x<<"\n";
	fout.close();
}

void decrypt(char *recp_priv_key, char *send_pub_key, char *text_file, char *output_file)
{
	ifstream fin;
	big_int d, N_recp, p, q, e, N_send, dp, dq, pmul, qmul;
	
	fin.open(recp_priv_key);
	fin>>d>>N_recp>>p>>dp>>pmul>>q>>dq>>qmul;
	fin.close();

	fin.open(send_pub_key);
	fin>>e>>N_send;
	fin.close();

	string res = "";
	fin.open(text_file);
	getline(fin, res);
	fin.close();

	res = RSA_decrypt(res, d, N_recp, p, dp, pmul, q, dq, qmul, true);
	res = RSA_decrypt(res, e, N_send, p, dp ,pmul, q, dq, qmul, false);
	res = rtrim(res);
	int i = (int)res.length()-1;
	for(;i>=0;--i)
		if(res[i]==32)
			break;
	string text = res.substr(0, i);
	string vig_key = res.substr(i+1);
	text = vigenere(text, vig_key, false);
	
	ofstream fout(output_file);
	fout<<text<<"\n";
	fout.close();
}
