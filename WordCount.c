#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* WordCounter: A tool that counts words,
   characters, and lines from a given 
   text file
*/
void count( FILE* fp );

int main(int ac, char *av[]) {
	FILE *file;
	//Check for input
	//If there is no given argument, assume stdin
	if(ac == 1) {
		count( stdin );
	}
	//If there is a valid argument treat that as the input
	else if(ac == 2 && (file=fopen(*++av, "r")) != NULL ) {
		count( file );
		fclose(file);
	}
	//If no valid argument has been given, exit
	else {
		printf( "Please provide a file!\n" );
		exit(1);
	}
	return 0;
}

void count( FILE* fp ) {
	//Declare variables
	int chars = 0;
	int words = 0;
	int lines = 0;
	char character; 
	//Check if the ile has ended
	while( character != EOF ) {
		//Get the next character
		character = fgetc( fp );
		//Increase the character count
		chars ++;
		/*
		If the character is a new line, 
		increase the word and line counts
		*/
		if(character == '\n') {
			lines ++;
			words++;
		}
		/*
		If the character is a space, 
		increase the word count
		*/
		if(character == ' ') {
			words++;
		}
	}
	//Remove offset
	chars --;
	//Print results
	printf( "Amount of Characters: %d\n", chars );
	printf( "Amount of Words: %d\n", words );
	printf( "Amount of Lines: %d\n", lines );
}
