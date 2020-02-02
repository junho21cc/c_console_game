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
		" @",
		"@@@" 
	},
	{ // ㄱ형
		"@@",
		" @",
		" @"
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

int dx = 0;
int dy = 1;

int form_number;
int form_width = 0;
int form_height = 0;

// default => 보여주는 좌표값의 기본값이다.
int default_y = 4;
int default_x = 10;

// position => 테트리스 좌표값이다.
int position_y = 0;
int position_x = 3;

void make_block()
{
	 form_number = rand() % 7;
	 form_width = 0;
	 form_height = 0;

	 // 블럭 모양의 가로와 세로 길이 구하기
	 for (int i = 0; i < 5; i++)
	 {
		 for (int j = 0; j < 5; j++)
		 {
			 if (tetris_block[form_number][i][j] == '@')
			 {
				 if (form_width <= j + 1)
				 {
					 form_width = j + 1;
				 }
				 form_height = i + 1;
			 }
		 }
	 }
}

void input_keyboard()
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
		if (ch == 77)
		{// 오른쪽
			dx = 1;
		}
	}
}

void next_position_check()
{
	for (int i = 0; i < form_height; i++)
	{
		int is_dy_0 = 0;

		for (int j = 0; j < form_width; j++)
		{
			// 블럭의 @가 움직일 방향에 빈칸이 있는경우에만 움직일수 있도록 함.
			// 아래로 무조건 떨어지는 것, 좌우로 움직이는것은 떨어지는거에 영향을 받지 않는다.
			// 하지만 벽쪽에서는 dx가 0이 되야한다.
			if (tetris_block[form_number][i][j] == '@')
			{
				if (tetris[position_y + i][position_x + j + dx] != ' ')
				{
					dx = 0;
				}
				if (tetris[position_y + i + 1][position_x + j] == ' ')
				{
					dy = 1;
				}
				else
				{
					dy = 0;
					is_dy_0 = 1;
					break;
				}
			}
		}
		if (is_dy_0 == 1)
		{
			break;
		}
	}
}

void input_position()
{
	for (int i = 0; i < form_height; i++)
	{
		for (int j = 0; j < form_width; j++)
		{
			// 
			if (tetris_block[form_number][i][j] == '@')
			{
				tetris[position_y + i][position_x + j] = '@';
			}
		}
	}
}

void input_void()
{
	for (int i = 0; i <= form_height; i++)
	{ //  원래 있던 자리에 빈칸으로 채우기
		for (int j = 0; j <= form_width; j++)
		{
			if (tetris_block[form_number][i][j] == '@')
			{
				putsxy(default_x + position_x + j, default_y + position_y + i, " ");

			}
		}
	}
}

void show_move()
{
	for (int i = 0; i <= form_height; i++)
	{   // 움직일 자리로 움직임
		for (int j = 0; j <= form_width; j++)
		{
			if (tetris_block[form_number][i][j] == '@')
			{
				putsxy(default_x + position_x + j, default_y + position_y + i, "@");
			}
		}
	}
}


int main()
{
	// 함수로 바꾸기?
	// 라인 클리어 어떻게?
	//	--> 한줄 만들어지면  지우고 한줄씩 밑으로 내리자. 
	// 회전 하기 
	//   1. 중심점찾기
	//   2. 중심점 기준해서 시계방향으로 회전 시키기
	//   3. 최대 가로길이와 최대 세로 길이를 서로 바꾼다
	//   4. 모양을 바꾼다.

	gotoxy(0, 4);
	for (int i = 0; i < 20; i++)
	{
		// 띄어쓰기 10번임
		printf("          %s\n", tetris[i]);
	}

	int count = 1;

	make_block();

	for (;;)
	{
		// 못 움직이는경우에만 초기화 시킴.
		if (dy == 0)
		{
			// 블럭을 떨어뜨리기 위해서 원래 위치로 올린다. 
			position_y = 0;
			position_x = 4;
			make_block();
		}
		// 키보드가 눌리는지 아닌지 판단.
		if (_kbhit())  // dx 입력 부분
		{
			input_keyboard();
			// 키보드 입력받으면 움직여야한다.
			// 함수로 분할시켜서 동작해야한다.
			// (1)떨어지는것
			// (2)좌우로 움직이는것
			// (3)회전하는것 들을 일반화 해서 함수로 만들어야한다.

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

			gotoxy(default_x + position_x, default_y + position_y);

			// dy, dx 결정 부분
			next_position_check();

			// 공백 삽입
			input_void();

			// 다음 위치
			position_x = position_x + dx;
			position_y = position_y + dy;

			if (dy == 0)
			{   // 테트리스 좌표에 직접 넣기
				input_position();
			}
			// 보여주기
			show_move();
		}
		delay(200);
	}
	return 0;
}