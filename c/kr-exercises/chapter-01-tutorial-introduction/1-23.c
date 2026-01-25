/* Exercise 1-23. Write a program to remove all SLASHs from a C program. Don't
 * forget to handle quoted strings and character constants properly. C SLASHs
 * don't nest.
 */
/*** See The Cleaned Version at 1-23_clean ***/

#include <stdio.h>

typedef enum {
  NORMAL,        // normal code
  SLASH,         // seen '/', waiting for '*' o '/'
  BLOCKCOMM,     // inside /* ... */
  STAR,          // inside /* ..., visto '*', waiting for '/'
  LINECOMM,      // inside // ...
  STRING,        // inside "..."
  STRING_ESCAPE, // inside "...\", prossimo char è escaped
  CHAR,          // inside '...'
  CHAR_ESCAPE    // inside '...\', prossimo char è escaped
} States;

// Leaving some comments here and there
int main(void) {
  States state = NORMAL;
  int c;

  /* Block comment: should be removed */
  // Test cases in code:
  char *s1 = "hello";             // simple string
  char *s2 = "say \"hi\"";        // escaped quotes in string
  char *s3 = "/* not comment */"; // fake block comment
  char *s4 = "// also not";       // fake line comment
  char c1 = 'a';                  // simple char
  char c2 = '\'';                 // escaped apostrophe
  char c3 = '\\';                 // escaped backslash
  char c4 = '/';                  // slash as char, not comment
  int x = 10 / 2;                 // division, not comment
  int y = 5;                      /* inline */
  int z = 6;                      // comment between code

  /*
   * Multi-line
   * block comment
   */
  while ((c = getchar()) != EOF) {
    switch (state) {
    case NORMAL:
      // '/' could be a comment - do not pront, yet
      if (c == '/')
        state = SLASH;
      else if (c == '"') {
        putchar(c);
        state = STRING;
      } else if (c == '\'') {
        putchar(c);
        state = CHAR;
      } else
        putchar(c);
      break;

    case SLASH:
      // Now we now if it was a comment
      if (c == '*')
        state = BLOCKCOMM; // Block comment
      else if (c == '/')
        state = LINECOMM; // Line comment
      else {
        putchar('/'); // Just a '/', print it
        putchar(c);
        state = NORMAL;
      }
      break;

    case BLOCKCOMM:
      // Ignore everything until you see '*'
      if (c == '*')
        state = STAR;

      break;

    case LINECOMM:
      // Ignore everything until newline
      if (c == '\n') {
        putchar('\n');
        state = NORMAL;
      }
      break;

    case STAR:
      // seen '*', if '/' follows, the comment ends
      if (c == '/')
        state = NORMAL;
      else if (c != '*')
        state = BLOCKCOMM; // false alarm, go back to look for '*'
      // if c == '*', stay in STAR
      break;

    case STRING:
      putchar(c);
      if (c == '\\')
        state = STRING_ESCAPE; // next char is escaped
      else if (c == '"')
        state = NORMAL; // string's over

      break;

    case STRING_ESCAPE:
      putchar(c); // print escaped char without interpreting it
      state = STRING;
      break;

    case CHAR:
      putchar(c);
      if (c == '\\') {
        state = CHAR_ESCAPE;
      } else if (c == '\'')
        state = NORMAL;
      break;

    case CHAR_ESCAPE:
      putchar(c);
      state = CHAR;
      break;
    }
  }
  return 0;
}