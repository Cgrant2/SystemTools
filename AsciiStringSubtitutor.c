#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/* ASCII String Substitutor: an interactive command
   line text interface; Prints keystrokes as an
   ASCII value in octal, decimal, and hexadecimal
*/

int main(int ac, char *argv[]) {

	struct termios info;
	char character;
	char opttest;
	int option = 3;
	char *mode = "HEXADECIMAL";

	if( tcgetattr(0, &info) == -1) {					 //Retrieve attributes
		perror("tcgetattr");
		exit(1);
	}

	info.c_lflag &= ~(ECHO | ICANON);					 //Turn off echo and canon bits
	if( tcsetattr(0, TCSANOW, &info) == -1) {
		perror("tcsetattr");
		exit(2);
	}

	while( (opttest = getopt(ac, argv, "odxX")) != -1) {
		switch(opttest) {
			case 'o':
				option = 0;
				mode = "Octal";
				break;
			case 'd':
				option = 1;
				mode = "Decimal";
				break;
			case 'x':
				option = 2;
				mode = "hexdecimal";
				break;
			case 'X':
				option = 3;
				mode = "HEXDECIMAL";
				break;
		}
	}
	
	fprintf(stdout, "Printing in: %s\nPress ESC to exit\n", mode);

	character = fgetc(stdin);							 //Get initial character
	while(character != 27) {
		if( character == '\n' ) {
			fprintf(stdout, "\n");						 //Print newlines normally
		}
		else if( character == 0x7F ) {
			fprintf(stdout, "\b\b\b\b\b    \b\b\b\b");
		}
		else if( character == 0xA ) {
		}
		else {
			switch(option) {
				case 0:
					fprintf(stdout, "%4o ", character);	 //Print the input in octal ASCII
					break;
				case 1:
					fprintf(stdout, "%4d ", character);	 //Print the input in decimal ASCII
					break;
				case 2:
					fprintf(stdout, "0x%x ", character); //Print the input in hexadecimal ASCII
					break;
				case 3:
					fprintf(stdout, "0x%X ", character); //Print the input in HEXADECIMAL ASCII
					break;
			}
		}
		character = fgetc(stdin);						 //Get the intput
	}
	fprintf(stdout, "\n");								 //Print newlines to return 
														 //prompt to normal place

	info.c_lflag |= (ECHO | ICANON);					 //Turn on echo and canon bits
	if( tcsetattr(0, TCSANOW, &info) == -1) {
		perror("tcsetattr");
		exit(2);
	}
}
