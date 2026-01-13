#include <stdio.h>

#define MAXLENGTH 100 
void to_uppercase(char s[]){
	int i = 0;
	while(s[i]){
		if(s[i] >= 'a' && s[i] <= 'z'){
			s[i] -= 32;
		}
		i++;
	}

}

int get_string(char s[], int max){
	int i,c ;
	i = 0;
	while(i < max - 1 && (c = getchar()) != EOF && c != '\n'){
		s[i] = c;
		i++;
	}
	s[i] = 0;

	return (c == EOF && i == 0) ? 0 : i;

}
int main(void){
	char string[MAXLENGTH];
	int len;
	while((len = get_string(string, MAXLENGTH)) > 0) {
		to_uppercase(string);
		printf("%s\n",string);		
	}

	return 0;
}
