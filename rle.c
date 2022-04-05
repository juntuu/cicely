/* Run length encoding */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rle()
{
	int n = 0;
	int p = EOF;
	for (int c; (c = getchar()) != EOF;) {
		if (c == p && n < 0xff) {
			n++;
		} else if (p != EOF) {
			printf("%c%c", p, n);
			n = 0;
		}
		p = c;
	}
	if (p != EOF)
		printf("%c%c", p, n);
}

void irle()
{
	for (int n, c; (c = getchar()) != EOF && (n = getchar()) != EOF;)
		for (n++; n--;)
			putchar(c);
}

int main(int argc, char **argv)
{
	if (argc == 2 && strcmp(argv[1], "-i") == 0) {
		irle();
	} else {
		rle();
	}
	return 0;
}
