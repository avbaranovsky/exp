#include <ncurses.h>
#include <string.h>

int main()
{
	char str[]="Hello!";
	int r, c;
	initscr();
	getmaxyx(stdscr, r, c);
	mvprintw(r/2, (c-strlen(str))/2, "%s", str);
	mvprintw(r-1,0,"Rows: %d, Col: %d\n", r,c);
	refresh();
	getch();
	endwin();

	return 0;
}

