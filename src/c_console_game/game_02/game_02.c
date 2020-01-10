﻿#include <stdio.h>
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
	{ // ㅣ형
		"@",
		"@",
		"@",
		"@"
	},
	{ // ㅁ형
		"@@",
		"@@"
	},
	{ //L 형
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
int position_y = 4;

int dx;
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
	clock_t check_point_1;

	gotoxy(0, position_y);
	for (int i = 0; i < 20; i++)
	{
		// 띄어쓰기 10번임
		printf("          %s\n", tetris[i]);
	}

	make_block();

	int count = 0;

	for (;;)
	{
		if (_kbhit())
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

			count++;

			if (count == 3)
			{// 아래
				count = 0;
				if (tetris[position_y - form_height - 1][1] == ' ')
				{
					for (int i = 0; i < form_height; i++)
					{
						putsxy(13 + dx, position_y - 1, "   ");
					}
					for (int i = 0; i < form_height; i++)
					{
						gotoxy(13 + dx, position_y + i);
						printf("%s\n", tetris_block[form_number][i]);
					}
					// 한칸씩 밑으로
					position_y = position_y + 1;
				}
				else
				{
					position_y = 4;
					make_block();
				}
			}
			delay(100);
		}
	return 0;
}