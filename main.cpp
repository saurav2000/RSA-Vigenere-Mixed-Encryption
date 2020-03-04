#include "CA.h"
#include "client.h"
using namespace std;

int main(int argc, char const *argv[])
{
	gmp_randclass rand_gen(gmp_randinit_mt);
	rand_gen.seed(time(NULL));

	generate_key(rand_gen, 512, "A_pub", "A_priv");
	generate_key(rand_gen, 512, "B_pub", "B_priv");
	encrypt("vig", "A_priv", "B_pub", "tex", "encrypted");
	decrypt("B_priv", "A_pub", "encrypted", "decrypted");
	return 0;
}