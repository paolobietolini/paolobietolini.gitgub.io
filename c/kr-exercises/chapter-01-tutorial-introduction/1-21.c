/*
 * Write a program entab that replaces strings of blanks by the minimum number
 * of tabs and blanks to achieve the same spacing. Use the same tab stops, say
 * every n columns. Should n be a variable or a symbolic parameter?
 */

#include <stdio.h>
#define TAB 8

void flush_spaces(int *spaces) {
  for (int i = 0; i < *spaces; i++) {
    putchar(' ');
  }
  *spaces = 0;
}

int dist_to_tab(int current_col) { return TAB - (current_col % TAB); }

int main(void) {
  int col = 0, spaces = 0, c;

  while ((c = getchar()) != EOF) {
    if (c == ' ') {
      spaces++;
      col++;
      if (col % TAB == 0) {
        putchar('\t');
        spaces = 0;
      }
    } else {
      flush_spaces(&spaces);
      putchar(c);
      if (c == '\n')
        col = 0;
      else if (c == '\t')
        col += dist_to_tab(col);
      else
        col++;
    }
  }

  flush_spaces(&spaces);

  return 0;
}