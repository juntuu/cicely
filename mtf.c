/* Move-to-front transform */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ALPHABET_SIZE 0x100

int mtf(int c, char* abc, int len)
{
	int pos = 0;
	for (; pos < len && c != abc[pos]; pos++)
		;

	assert((pos >= 0) && (pos < len));

	for (int i = pos; i > 0; i--)
		abc[i] = abc[i-1];
	abc[0] = c;
	return pos;
}

int imtf(int pos, char* abc, int len)
{
	assert(pos >= 0 && pos < len);

	int c = abc[pos];
	for (int i = pos; i > 0; i--)
		abc[i] = abc[i-1];
	abc[0] = c;
	return c;
}

int main(int argc, char **argv)
{
	int (*f)(int, char*, int) = mtf;
	char defabc[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; i++)
		defabc[i] = i;

	char *abc = defabc;
	int len = ALPHABET_SIZE;
	if (argc > 1) {
		if (strcmp(argv[1], "-i") == 0) {
			f = imtf;
			if (argc == 3) {
				abc = argv[2];
				len = strlen(abc);
			}
		} else {
			abc = argv[1];
			len = strlen(abc);
		}
	}
	for (int c; (c = getchar()) != EOF;)
		putchar(f(c, abc, len));

	return 0;
}
