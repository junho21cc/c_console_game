#include <stdio.h>
#if !defined(__APPLE__)
#include <conio.h>
#endif
#include "cursor.h"

#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define KEY_ESC 27

int main()
{
	clrscr();
	putsxy(45, 2, "KEY TEST");
	putsxy(45, 4, "ESC:종료");
	putsxy(55, 10, "A");

	// 게임 진행 루프
	int x = 55;
	int y = 10;

	for (;;)
	{
		int ch;	
		ch = getch();
		if (ch == 0xE0 || ch == 0)
		{
			ch = getch();
			
			if (ch == 72)
			{
				putsxy(x, y - 1, "A");
				putsxy(x, y, " ");
				y = y - 1;
				printf("(%d, %d)", x, y);
			}
			else if(ch == 80)
			{
				putsxy(x, y + 1, "A");
				putsxy(x, y, " ");
				y = y + 1;
				printf("(%d, %d)", x, y);
			}
			else if (ch == 75)
			{
				putsxy(x - 1, y, "A");
				putsxy(x, y, " ");
				x = x - 1;
				printf("(%d, %d)", x, y);
			}
			else
			{
				putsxy(x + 1, y, "A");
				putsxy(x, y, " ");
				x = x + 1;
				printf("(%d, %d)", x, y);
			}
			/*
			if (x < 0)
			{
				x = 0;
			}
			if(x > 118 || y > 28)
			*/
		}
		else
		{
			if (ch == KEY_ESC)
			{
				break;
			}
		}
	}

	return 0;
}