# RSA-Vigenere-Mixed-Encryption

## Before running the code
- Create 2 files named `vig` and `tex`. **Do not use extensions like .txt**
- The vignere key should be entered in the `vig` file. The key should contain **only lowercase letters and digits**.
- The `tex` file should contain the text to be encrypted. **Special characters other than comma, apostrophe and full-stop will most be ignored.**

## Running the code
- Compile the code using the command `make all`.
- Run the code using `./main`.

## Files generated
- `A_priv` and `A_pub` are the private and public keys being generated for user A and similarly for B.
- `encrypted` is the file with the encrypted text after the Vigenere and RSA encryption.
- `decrypted` is the file with the text after decryption.

## Code Explanation
- The file `CA.cpp` acts as the Certificate Authority and generates the keys using functions for Miller-Rabin test, extended GCD, Chinese Remainder Theorem and Strong Prime Generators.
- The file `client.cpp` has functions which encrypts / decrypts for RSA / vigenere ciphers and performs them accordingly.
- The file `main.cpp` generates 2 set of keys with 512bit primes and then uses the `vig` and `tex` files and calls the encrypt function from `client.cpp` which generates the `encrypted` file and then immediately calls the decrypt function from `client.cpp` which generates the `decrypted` file.
