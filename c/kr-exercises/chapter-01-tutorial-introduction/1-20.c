/*
 * Write a program detab that replaces tabs in the input with the proper number of blanks to space to the next tab stop. 
 * Assume a fixed set of tab stops, say every n columns. Should n be a variable or a symbolic parameter?
 */

#include <stdio.h>
#define TAB 8
int main(void) {
	int i, spaces, col;
	spaces = col = 0;
	char c;
	while((c = getchar()) != EOF) {
		if(c == '\t') {
			spaces = TAB - (col % TAB);
			col += spaces;
			for(i = 0; i < spaces; i++)
				printf(" ");
		} else {
			printf("%c",c);
			col++;
		}
		if (c == '\n')
			col = 0;
	}
	return 0;
}
