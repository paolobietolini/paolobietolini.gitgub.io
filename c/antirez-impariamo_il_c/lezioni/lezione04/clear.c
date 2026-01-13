#include <stdio.h>


void clear(void) {
    printf("\033[H\033[2J\033[3J");
}


int main(void) {
    
    clear(); // Pulisce il terminale
    printf("Hello World\n");
    clear();
   return 0;
}