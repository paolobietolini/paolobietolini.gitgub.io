/**
 * Compila con gcc -O0 -fno-stack-protector -fno-omit-frame-pointer -g garbage.c -o garbage && ./garbage
 * oppure con cc -O3 -Wall garbage.c -o garbage && ./garbage
 */
#include <stdio.h>


int main(void){
	char junk[32];
	char s[] = {'h','e','l','l','o'};
	int b = 45;
	printf("%s\n", s);
	for (int i = 0; i < 40; i++)
		printf("%02x(%c) ", (unsigned char)s[i], s[i]);
	printf("\n");

	return 0;
}
