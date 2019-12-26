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
	char stage[20][40] = { 
		"####", 
		"#  #",
		"#  #",
		"#  ###",
		"#.$$@#",
		"#  . #",
		"#  ###", 
		"####" };
	// 가변 스테이지
	char back_stage[20][40];

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
			back_stage[i][j] = stage[i][j];
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
			else if (back_stage[y + dy][x + dx] == ' ' || back_stage[y + dy][x + dx] == '.')
			{

			}
			else// 진행방향 1칸 앞에 상자인 경우
			{
				// 진행방향 2칸 앞에 상자가 아닌경우
				if (back_stage[y + dy * 2][x + dx * 2] == ' ' || back_stage[y + dy * 2][x + dx * 2] == '.')
				{
					putsxy(x + dx * 2, y + dy * 2, "$");
					back_stage[y + dy * 2][x + dx * 2] = '$';
				}
				// 진행방향 2칸 앞에 상자인 경우
				else
				{
					dx = 0;
					dy = 0;
				}
			}
			// 주인공 움직이기
			putsxy(x + dx, y + dy, "@");
			back_stage[y + dy][x + dx] = '@';

			// 지나온 자리
			if (dx != 0 || dy != 0)
			{
				putsxy(x, y, " ");
				back_stage[y][x] = ' ';
				// 점 살리기
				if (stage[y][x] == '.')
				{
					putsxy(x, y, ".");
				}
			}
			x = x + dx;
			y = y + dy;
		}
	}

	return 0;
}