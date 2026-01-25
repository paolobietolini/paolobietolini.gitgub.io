
#include <stdio.h>

typedef enum {
  NORMAL,        
  SLASH,         
  BLOCKCOMM,     
  STAR,          
  LINECOMM,      
  STRING,        
  STRING_ESCAPE, 
  CHAR,          
  CHAR_ESCAPE    
} States;


int main(void) {
  States state = NORMAL;
  int c;

  
  
  char *s1 = "hello";             
  char *s2 = "say \"hi\"";        
  char *s3 = "/* not comment */"; 
  char *s4 = "// also not";       
  char c1 = 'a';                  
  char c2 = '\'';                 
  char c3 = '\\';                 
  char c4 = '/';                  
  int x = 10 / 2;                 
  int y = 5;                      
  int z = 6;                      

  
  while ((c = getchar()) != EOF) {
    switch (state) {
    case NORMAL:
      
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
      
      if (c == '*')
        state = BLOCKCOMM; 
      else if (c == '/')
        state = LINECOMM; 
      else {
        putchar('/'); 
        putchar(c);
        state = NORMAL;
      }
      break;

    case BLOCKCOMM:
      
      if (c == '*')
        state = STAR;

      break;

    case LINECOMM:
      
      if (c == '\n') {
        putchar('\n');
        state = NORMAL;
      }
      break;

    case STAR:
      
      if (c == '/')
        state = NORMAL;
      else if (c != '*')
        state = BLOCKCOMM; 
      
      break;

    case STRING:
      putchar(c);
      if (c == '\\')
        state = STRING_ESCAPE; 
      else if (c == '"')
        state = NORMAL; 

      break;

    case STRING_ESCAPE:
      putchar(c); 
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