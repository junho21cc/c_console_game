#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <curses.h>
#endif

#include <time.h>

#define randomize() srand((unsigned)time(NULL))
#define random(n) (rand() % (n))
#define delay(n) Sleep(n)

#if defined(_WIN32) || defined(_WIN64)
#define clrscr() system("cls")
#define gotoxy(x,y) { COORD Cur = {x, y}; \
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);}
#define showcursor(bShow) { CONSOLE_CURSOR_INFO CurInfo = {20, bShow}; \
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo); }
#define getch _getch

#else
#define clrscr() system("clear")
#define gotoxy(x,y) { printf("\033[%d;%df", y, x); fflush(stdout); }
#endif