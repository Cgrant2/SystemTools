#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>

/*
  Snake Go Move: A basic CLI based clone
  of the classic game snake
  ============================================================
  Compile with: 'gcc -Wall -std=gnu18 -o sgmA sgmA.c -lcurses'
  ============================================================
*/
#define WALLSCOLOR 1
#define SNAKECOLOR 2
#define APPLECOLOR 3

int main(int argc, char **argv) {
	//display help text
	if(argc == 2 || argc > 3 ) {
		fprintf(stdout, "Snake Go Move:\n");
		fprintf(stdout, "==============\n");
		fprintf(stdout, "A basic snake clone.\n");
		fprintf(stdout, "Move with WASD.\n");
		fprintf(stdout, "Takes 2 arguments for\n");
		fprintf(stdout, "the play area: int width,\n");
		fprintf(stdout, "int height. If no argument\n");
		fprintf(stdout, "is given size defaults to fit\n");
		fprintf(stdout, "the terminal\n");
		return 0;
	}

	//general setup
	srand(time(NULL));
	struct Snake {
		int x;
		int y;
		int dir;		//1:right -1:left 2:down -2:up
	};
	int breaker = 1;

	//Variable playbound
	initscr();
	start_color();
	init_pair(WALLSCOLOR, COLOR_YELLOW, 0);
	init_pair(SNAKECOLOR, COLOR_GREEN, 0);
	init_pair(APPLECOLOR, COLOR_RED, 0);
	int LEFTEDGE = 1;
	int TOP	= 1;
	int RIGHTEDGE = COLS-LEFTEDGE-1;
	int BOTTOM = LINES - TOP;
	if(argc == 3) {
		RIGHTEDGE = atoi(argv[1]) + LEFTEDGE;
		BOTTOM	= atoi(argv[2]) + TOP;
	}

	//game variables
	int i;
	int tail = 2;
	int  applex  = (rand() % (RIGHTEDGE-LEFTEDGE-1)+LEFTEDGE+1);
	int  appley = (rand() % (BOTTOM-TOP-1)+TOP+1);
	struct Snake snake[300];
	int key;
	char points[12] = "0";
	int delay = 70000;

	//death handler
	//the name speaks for itself
	void dead() {
		//Remake snake
		snake[0].x		= LEFTEDGE + 3;
		snake[0].y		= (BOTTOM-TOP)/2;
		snake[0].dir	= 1;
		snake[1].x		= LEFTEDGE + 2;
		snake[1].y		= (BOTTOM-TOP)/2;
		snake[1].dir	= 1;
		snake[2].x		= LEFTEDGE + 1;
		snake[2].y		= (BOTTOM-TOP)/2;
		snake[2].dir	= 1;
		tail = 2;

		sleep(1);
		//Delete and remake apple
		move(appley, applex);
		addstr(" ");
		applex  = (rand() % (RIGHTEDGE-LEFTEDGE-1)+LEFTEDGE+1);
		appley = (rand() % (BOTTOM-TOP-1)+TOP+1);
		//Print starting points
		move(0,LEFTEDGE+1);
		addstr("Points:0   ");
		return;
	}

	//initialize an empty snake
	
	for(i=0; i<=300; i++) {
		snake[i].x = 0;
		snake[i].y = 0;
		snake[i].dir = 0;
	}
	
	//Initialize snake head
	snake[0].x		= LEFTEDGE + 3;
	snake[0].y		= (BOTTOM-TOP)/2;
	snake[0].dir	= 1;

	//initialize 2 snake nodes
	snake[1].x		= LEFTEDGE + 2;
	snake[1].y		= (BOTTOM-TOP)/2;
	snake[1].dir	= 1;

	snake[2].x		= LEFTEDGE + 1;
	snake[2].y		= (BOTTOM-TOP)/2;
	snake[2].dir	= 1;

	//Starting game
	clear();
	//Draw borders
	move(0,(RIGHTEDGE-LEFTEDGE)/2+LEFTEDGE);
	addstr("Press q to exit");
	attron(COLOR_PAIR(WALLSCOLOR));
	for(i=LEFTEDGE; i<=RIGHTEDGE; i++) {
		move( TOP, i );
		addstr( "=" );
		move( BOTTOM, i );
		addstr( "=" );
	}
	for(i=TOP; i<=BOTTOM; i++) {
		move( i, LEFTEDGE );
		addstr( "|" );
		move( i, RIGHTEDGE);
		addstr( "|" );
	}
	attroff(COLOR_PAIR(WALLSCOLOR));
	//Print starting points
	move(0,LEFTEDGE+1);
	addstr("Points:0   ");
	while(breaker) {
		curs_set(0);
		//Draw snake
		move(snake[0].y, snake[0].x);
		attron(COLOR_PAIR(SNAKECOLOR));
		addstr("S");
		for(i=1; i<=tail; i++) {
			move(snake[i].y, snake[i].x);
			addstr("s");
		}
		attroff(COLOR_PAIR(SNAKECOLOR));
		//Draw apple
		move(appley, applex);
		attron(COLOR_PAIR(APPLECOLOR));
		addstr("A");
		attroff(COLOR_PAIR(APPLECOLOR));
		refresh();
		usleep(delay);
		delay = 70000;
		//Get user input
		move( 0, 0 );
		nodelay(stdscr, 1);
		keypad(stdscr, 1);
		noecho();
		key = wgetch(stdscr);
		//change snake head's direction
		switch(key) {
			case 'w':
			case 'W':
			case KEY_UP:
				if(snake[0].dir != 2) {
					snake[0].dir = -2;
				}
				break;
			case 'a':
			case 'A':
			case KEY_LEFT:
				if(snake[0].dir != 1) {
					snake[0].dir = -1;
				}
				break;
			case 's':
			case 'S':
			case KEY_DOWN:
				if(snake[0].dir != -2) {
					snake[0].dir = 2;
				}
				break;
			case 'd':
			case 'D':
			case KEY_RIGHT:
				if(snake[0].dir != -1) {
					snake[0].dir = 1;
				}
				break;
			case ' ':
				delay = 5000;
				break;
			case 27:
			case 'q':
			case 'Q':
				breaker = 0;
				break;
			default:
				snake[0].dir = snake[0].dir;
				break;
		}
		//delete previous locations
		for(i=0; i<=tail; i++) {
			move(snake[i].y, snake[i].x);
			addstr(" ");
		}
		//update each segment
		for(i=tail; i>0; i--) {
			snake[i].x = snake[i-1].x;
			snake[i].y = snake[i-1].y;
		}
		//update head direction
		switch(snake[0].dir) {
			case -1:
				snake[0].x--;
				break;
			case 1:
				snake[0].x++;
				break;
			case -2:
				snake[0].y--;
				break;
			case 2:
				snake[0].y++;
				break;
		}
		//apple get
		if(snake[0].x == applex && snake[0].y == appley) {
			tail++;
			applex  = (rand() % (RIGHTEDGE-LEFTEDGE-1)+LEFTEDGE+1);
			appley = (rand() % (BOTTOM-TOP-1)+TOP+1);
			move(0,LEFTEDGE+1);
			addstr("Points:");
			move(0,LEFTEDGE+8);
			sprintf(points,"%d",(tail-2));
			addstr(points);
		}
		//screen bound hit
		if(snake[0].x >= RIGHTEDGE || snake[0].x <= LEFTEDGE || snake[0].y >= BOTTOM || snake[0].y <= TOP) {
			dead();
		}
		//snake segment hit
		for(i=1; i<=tail; i++) {
			if(snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
				dead();
			}
		}

	}
	nodelay(stdscr, 0);
	keypad(stdscr, 0);
	echo();
	endwin();
}

