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

	// 게임 진행 루프
	for (;;)
	{
		int ch;
		ch = _getch();
		printf("[1]key = %d, 0x%X\n", ch, ch);
		if (ch == 0xE0 || ch == 0)
		{
			ch = _getch();
			printf("[2]key = %d, 0x%X\n", ch, ch);
			/*
			switch (ch)
			{
				case 
			}
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