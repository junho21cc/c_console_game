#include <stdio.h>
#include <time.h>
#if !defined(__APPLE__)
#include <conio.h>
#endif
#include "cursor.h"

#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define KEY_ESC 27

char tetris[21][11] = {
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"#        #",
	"##########",
};

char tetris_block[10][5][5] =
{
	{ // I형
		"@",
		"@",
		"@",
		"@"
	},
	{ // ㅁ형
		"@@",
		"@@"
	},
	{ // L형
		"@",
		"@",
		"@@"
	},
	{ // ㅗ형
		"@",
		"@@",
		"@"
	},
	{ // ㄱ형
		"@@@",
		"@"
	},
	{ // Z형
		"@@",
		" @@"
	},
	{ // S형
		" @@",
		"@@"
	}
};
int form_number;
int form_width = 0;
int form_height = 0;

void make_block()
{
	 form_number = rand() % 7;

	 // 블럭 모양의 가로와 세로 길이 구하기
	 for (int i = 0; i < 5; i++)
	 {
		 for (int j = 0; j < 5; j++)
		 {
			 if (tetris_block[form_number][i][j] == NULL)
			 {
				 if (form_width <= j)
				 {
					 form_width = j;
				 }
				 break;
			 }
		 }
		 if (tetris_block[form_number][i][0] == NULL)
		 {
			 form_height = i;
			 break;
		 }
	 }
}


int main()
{
	int dx = 0;
	int dy = 1;
	
	gotoxy(0, 4);
	for (int i = 0; i < 20; i++)
	{
		// 띄어쓰기 10번임
		printf("          %s\n", tetris[i]);
	}

	int position_y = 4;
	int position_x = 13;

	int count = 1;

	make_block();

	for (;;)
	{
		// 못 움직이는경우에만 초기화 시킴.
		if (dy == 0)
		{
			position_y = 4;
			position_x = 13;
			make_block();
		}

		if (_kbhit())  // dx 결정 부분
		{
			int ch;
			ch = getch();
			if (ch == 0xE0 || ch == 0)
			{
				ch = getch();
				/*
				if (ch == 72)
				{// 위
					dy = -1;
				}
				*/
				if (ch == 75)
				{// 왼쪽
					dx = -1;
				}
				else
				{// 오른쪽
					dx = 1;
				}
			}
		}
		else
		{
			dx = 0;
		}

		count++;

		if (count == 3)
		{
			count = 1;
			/*
			if (tetris[position_y - form_height - 1][1] == ' ')
			{
				for (int i = 0; i < form_height; i++)
				{
					putsxy(position_x, position_y - 1, "     ");
				}
				for (int i = 0; i < form_height; i++)
				{
					gotoxy(position_x+ dx, position_y + i);
					printf("%s\n", tetris_block[form_number][i]);
				}
				// 한칸씩 밑으로
				position_y = position_y + 1;
				position_x = position_x + dx;
			}
			else
			{
				position_y = 4;
				make_block();
			}
			*/
			/*
			블럭이 아래로 떨어지는 알고리즘
			1. 블럭들의 현재위치에 X좌표, Y좌표를 구한다
			2. 블럭안에 @들이 떨어지려는 방향에  움직이려는 칸이 빈칸이여만 움직임
				낙하는 무조건 하는 일
			3. 움직이려는 방향의 움직이려는 칸에 #이나 @이 존재하면 안됨 !!
			4. 각 블럭의 인덱스마다 비교를 해야됨. (그리기만 함, TETRIS에 직접 적용하지 않음)
			*/

			gotoxy(position_x, position_y);

			// dy 결정 부분

			for (int i = 0; i < form_height; i++)    
			{
				for (int j = 0; j < form_width; j++)
				{
					// 블럭의 @가 움직일 방향에 빈칸이 있는경우에만 움직일수 있도록 함.
					// 아래로 무조건 떨어지는 것, 좌우로 움직이는것은 떨어지는거에 영향을 받지 않는다.
					if (tetris_block[form_number][i][j] == '@')
					{
						if (tetris[position_y + i - 3][position_x + j - 11] == ' ')
						{
							dy = 1;
						}
						else
						{
							dy = 0;
							break;
						}
					}
				}
			} 
				// 움직이기

			if (dy == 0)
			{   // 테트리스 좌표에 직접 넣기
				for (int i = 0; i < form_height; i++)
				{
					for (int j = 0; j < form_width; j++)
					{
						// 
						if (tetris_block[form_number][i][j] == '@')
						{
							tetris[position_y - 4][position_x - 11] = '@';
						}
					}
				}
			}

			for (int i = 0; i < form_height; i++)
			{ //  원래 있던 자리에 빈칸으로 채우기
				for (int j = 0; j < form_width; j++)
				{
					putsxy(position_x + j, position_y + i, " ");
					
				}
			}

			for (int i = 0; i < form_height; i++)
			{   // 움직일 자리로 움직임
				putsxy(position_x + dx, position_y + dy + i, tetris_block[form_number][i]);
			}
			// 다음 위치
			position_x = position_x + dx;
			position_y = position_y + dy;


		}
		delay(50);
	}
	return 0;
}
