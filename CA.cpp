#include "CA.h"

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

void generate_key(int bit_count, char *public_file, char *private_file)
{
	big_int b = bit_count;
	gmp_randclass rand_gen(gmp_randinit_mt);
	rand_gen.seed(time(NULL));
	generate_key(rand_gen, b, public_file, private_file);
}