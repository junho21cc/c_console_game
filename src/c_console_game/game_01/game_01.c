#include <stdio.h>
#if !defined(__APPLE__)
#include <conio.h>
#endif
#include "cursor.h"

#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define KEY_ESC 27

int main()
{
	// 고정된 스테이지
	char stage[20][40] = { "####", "#  #", "#  #",
		"#  ###", "#.$$@#", "#  . #", "#  ###", "####" };
	// 가변 스테이지
	char back_stage[20][40] = { "####", "#  #", "#  #",
		"#  ###", "#.$$@#", "#  . #", "#  ###", "####" };

	int x, y;
	int dx, dy;

	// 스테이지 출력( 기본값 )
	for (int i = 0; i < 20; i++)
	{
		printf("%s\n", stage[i]);
	}

	putsxy(45, 2, "STAGE");

	// 주인공 찾기
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			if (stage[i][j] == '@')
			{
				x = j;
				y = i;
			}
		}
	}
	// 게임 루프
	for (;;)
	{
		dx = 0;
		dy = 0;
		int ch;	

		// 방향키 정보 알아내기
		ch = getch();
		if (ch == 0xE0 || ch == 0)
		{
			ch = getch();
			
			if (ch == 72)
			{
				dy = -1;
			}
			else if(ch == 80)
			{
				dy = 1;
			}
			else if (ch == 75)
			{
				dx = -1;
			}
			else
			{
				dx = 1;
			}
			// 벽 판단
			if (stage[y + dy][x + dx] == '#')
			{
				dx = 0;
				dy = 0;
			}
			// 움직이는 상자 판단
			else if (back_stage[y + dy][x + dx] == '$')
			{
				if (back_stage[y + dy*2][x + dx*2] != ' ')
				{
					// 위치에 도달한 상자는 만족됨
					if (stage[y + dy*2][x + dx*2] == '.')
					{
						back_stage[y + dy][x + dx] = 'ㅇ';
					}
					else
					{
						dx = 0;
						dy = 0;
					}
				}
				putsxy(x + dx * 2, y + dy * 2, "$");
				back_stage[y + dy * 2][x + dx * 2] = '$';
			}

			putsxy(x + dx, y + dy, "@");
			back_stage[y + dy][x + dx] = '@';
			if (dx != 0 || dy != 0)
			{
				putsxy(x, y, " ");
				back_stage[y][x] = ' ';
			}
			if (stage[y][x] == '.')
			{
				putsxy(x, y, ".");
			}

			x = x + dx;
			y = y + dy;
		}
	}

	return 0;
}