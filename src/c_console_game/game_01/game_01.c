#include <stdio.h>
#if !defined(__APPLE__)
#include <conio.h>
#endif
#include "cursor.h"

#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define KEY_ESC 27

int stage_level = 0;
int stage_width = 0;
int stage_height = 0;

// 고정된 스테이지
char stage[5][20][40] = {
{
	// 레벨 1
	"####",
	"#  #",
	"#  #",
	"#  ###",
	"#.$$@#",
	"#  . #",
	"#  ###",
	"####"
},
{
	// 레벨 2
	" #####",
	" #   #",
	"##.# #",
	"#  @ #",
	"#  $ #",
	"# # ##",
	"#   #",
	"#####"
},
{
	"   ####",
	"####  #",
	"#  #  #",
	"# . . #",
	"# @$$ #",
	"# # ###",
	"#   #",
	"#####"
},
{
	" #####",
	"## @ ####",
	"#  #  . ##",
	"# #      #",
	"# $$ #.  #",
	"##    ####",
	" ##   #",
	"  #####"
},
{
	"######",
	"#    #",
	"# $$ ###",
	"### @  #",
	"  #  . #",
	"  ## .##",
	"   ####"
},
};
// 가변 스테이지
char back_stage[20][40];
int x, y;
int moving_count;

void init_stage()
{
	clrscr();

	char buff[10];
	sprintf_s(buff, sizeof(buff), "%d", stage_level + 1);

	putsxy(60, 2, "S T A G E ");
	putsxy(70, 2, buff);
	putsxy(60, 4, "다시 하기 : r");
	putsxy(60, 5, "끝내기 : x");
	putsxy(60, 6, "움직이기 : 방향키");

	// 주인공 찾기
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			if (stage[stage_level][i][j] == '@')
			{
				x = j;
				y = i;
			}
			back_stage[i][j] = stage[stage_level][i][j];
			if (stage[stage_level][i][j] == NULL)
			{
				if (stage_width <= j)
				{
					stage_width = j;
				}
				break;
			}
		}
		if (stage[stage_level][i][0] == NULL)
		{
			stage_height = i;
			break;
		}
	}
	moving_count = 0;

	// 스테이지 출력( 기본값 )

	for (int i = 0; i < 20; i++)
	{
		gotoxy((60 - stage_width) / 2, (24 - stage_height) / 2 + i);
		printf("%s\n", stage[stage_level][i]);
	}
}


int main()
{
	init_stage();

	int dx, dy;
	int is_win;

	
	// 게임 루프
	for (;;)
	{
		is_win = 1;
		dx = 0;
		dy = 0;
		int ch;	

		// 방향키 정보 알아내기
		ch = getch();
		if (ch == 0xE0 || ch == 0)
		{
			moving_count++;
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
			if (stage[stage_level][y + dy][x + dx] == '#')
			{
				moving_count--;
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
					putsxy(x + dx * 2 + (60 - stage_width) / 2, y + dy * 2 + (24 - stage_height) / 2, "$");
					back_stage[y + dy * 2][x + dx * 2] = '$';
				}
				// 진행방향 2칸 앞에 상자인 경우
				else
				{
					moving_count--;
					dx = 0;
					dy = 0;
				}
			}
			// 주인공 움직이기
			putsxy(x + dx + (60 - stage_width) / 2, y + dy + (24 - stage_height) / 2, "@");

			// 지나온 자리
			if (dx != 0 || dy != 0)
			{
				putsxy(x + (60 - stage_width) / 2, y + (24 - stage_height) / 2, " ");
				back_stage[y][x] = ' ';

				char mc[10];
				sprintf_s(mc, sizeof(mc), "%d", moving_count);

				putsxy(45, 4, "움직인횟수");
				putsxy(45, 5, mc);

				// 점 살리기
				if (stage[stage_level][y][x] == '.')
				{
					putsxy(x + (60 - stage_width) / 2, y + (24 - stage_height) / 2, ".");
					back_stage[y][x] = '.';

				}
			}
			x = x + dx;
			y = y + dy;
		}
		// 승리 조건
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 40; j++)
			{
				if (back_stage[i][j] == '.')
				{
					is_win = 0;
				}
			}
		}
		if (is_win == 1)
		{
			putsxy(45, 20, "you are winner!");
			stage_level++;
			init_stage();
		}

		// 새로하기
		if (ch == 'r' || ch == 'R')
		{
			init_stage();
		}

		// 종료하기
		if (ch == 'x'|| ch == 'X')
		{
			break;
		}
	}

	return 0;
}