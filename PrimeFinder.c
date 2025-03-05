#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>

/* PrimeFinder: an inefficient prime searching
   program; created to demonstrate the ability
   to modify command line signals usage;
   Provides a status report when given a SIGINT
*/  

int breaker = 1;
int interruptflag = 0;

int main() {
	struct termios info;
	int i = 3;
	int prime = 2;
	void safequit(int);
	void status(int);

	///Turn off terminal echoing
	if( tcgetattr(0, &info) == -1) {                     //Retrieve attributes
        perror("tcgetattr");
        exit(1);
    }

    info.c_lflag &= ~(ECHO | ICANON);                    //Turn off echo and canon bits
    if( tcsetattr(0, TCSANOW, &info) == -1) {
        perror("tcsetattr");
        exit(2);
    }
	///

	//Set up signal handlers
	signal( SIGINT, status );
	signal( SIGQUIT, safequit );
	
	//Main functionality; search for primes
	while(breaker) {
		if(interruptflag) {
			printf("Highest prime found: %d\n", prime);
			interruptflag = 0;
		}
		for(int j=3; j<=i; j+=2) {
			if( i % j == 0 ) {
				break;
			}
			if( j == i-2 ) {
				prime = i;
			}
		}
		i++;
	}
	
	//Return terminal to normal and print exit message
	info.c_lflag |= ECHO;
    if( tcsetattr(0, TCSANOW, &info) == -1) {
        perror("tcsetattr");
        exit(2);
    }
	fprintf(stdout, "Exiting program\n");
}
void status(int signum) {
	interruptflag = 1;
	return;
}
void safequit(int signum) {
	breaker = 0;
	return;
}
