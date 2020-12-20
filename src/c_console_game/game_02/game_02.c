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

// 블럭 형태, 회전 횟수, 세로, 가로
char tetris_block[7][4][5][5] =
{
	{
		{ // I형
			"@",
			"@",
			"@",
			"@"
		},
		{
			"@@@@"
		}
	},
	{
		{ // ㅁ형
			"@@",
			"@@"
		},
	},
	{
		{ // L형
			"@",
			"@",
			"@@"
		},
		{
			"@@@",
			"@"
		},
		{
			"@@",
			" @",
			" @"
		},
		{
			"  @",
			"@@@"
		}
	},
	{
		{ // ㅗ형
		" @",
		"@@@"
		},
		{ 
		"@",
		"@@",
		"@"
		},
		{
		"@@@",
		" @ "
		},
		{
		" @",
		"@@",
		" @"
		}
	},
	{
		{ // ㄱ형
			"@@",
			" @",
			" @"
		},
		{
			"  @",
			"@@@"
		},
		{
			"@",
			"@",
			"@@"
		},
		{
			"@@@",
			"@  "
		}
	},
	{
		{ // Z형
		"@@",
		" @@"
		},
		{
		" @",
		"@@",
		"@"
		}
	},
	{
		{ // S형
			" @@",
			"@@"
		},
		{
			"@",
			"@@",
			" @"
		}
	}
};

int dx = 0;
int dy = 1;
int dr = 0;

// 블럭에 관한 정보들이다.
int form_number;
int form_width = 0;
int form_height = 0;

// 회전 횟수이다.
int rotation_number = 0;

// default => 보여주는 좌표값의 기본값이다.
int default_y = 4;
int default_x = 10;

// position => 테트리스 좌표값이다.
int position_y = 0;
int position_x = 3;

// 7개의 무작위 모양 중에서 하나를 선택해 최대 가로 길이와 최대 세로 길이를 구한다.
void make_block()
{
	srand(time(NULL));
	 form_number = rand()%7;
	 form_width = 0;
	 form_height = 0;

	 // 블럭 모양의 가로와 세로 길이 구하기
	 for (int i = 0; i < 4; i++)
	 {
		 for (int j = 0; j < 4; j++)
		 {
			 if (tetris_block[form_number][rotation_number][i][j] == '@')
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
// 모양에 따라서 회전하는 주기가 다르기때문에  스위치를 사용해서 회전 주기를 맞춰준다.
void rotation_form_is_right()
{
	switch (form_number)
	{
		case 0:
		case 5:
		case 6:
			if (rotation_number > 0)
			{
				rotation_number = 0;
			}
			else if (rotation_number < 0)
			{
				rotation_number = 0;
			}
			break;
		case 2:
		case 3:
		case 4:
			if (rotation_number > 3)
			{
				rotation_number = 0;
			}
			else if (rotation_number < 0)
			{
				rotation_number = 3;
			}
			break;
		case 1:
			rotation_number = 0;
			break;
	}
}
// 회전시키는걸 함수를 하나 만들어서 비교한다
// 이전와 나중을 비교하는것 --> rotationumber를 나중과 이전, 1차이나는것을 비교한다.
// 좌우로 움직이는것과 아래로 떨어지면서 모양이 바뀌는거니까 따로 비교한다.
// rn이 바뀐다는것은 최대 가로길이와 세로 길이가 바뀐다는것이다.
// deltarotationumber 이 0이될 조건 --> 나중모양이 위치할 곳에 빈칸이 있으면 가능, 한개라도 없으면 불가능

// 키보드를 입력받을때, 어떤 키보드가 입력이 되었는지에 따라서 좌우로 움직이거나 회전한다.
void input_keyboard()
{
	int ch;
	ch = getch();
	dr = dx = dy = 0;
	if (ch == 0xE0 || ch == 0)
	{
		ch = getch();
		switch (ch)
		{
		case 72:
			dr = 1;
			break;
		case 75:
			dx = -1;
			break;
		case 77:
			dx = 1;
			break;
		}
	}
}

// dr이 
void rotation_form()
{
	int A = form_width;
	form_width = form_height;
	form_height = A;

	rotation_form_is_right();
}

// 테트리스 블럭의 @이가 있는 부분이 움직일때 움직이려는 칸이 빈칸이여야 하고, 빈칸이 아니라면 dx나 dy가 0이 된다.
// 회전된 경우에 빈칸이 아닌 다른 문자가 있다면, 회전을 하면 안된다. 

void next_form_check()
{
	// 내생각에는 dr이 0보다 작은경우와 큰경우 모두 같은 함수로 가는게 맞다고 생각한다.
	// rotation_number = rotation_number + dr;
	if (dr != 0)
	{
		rotation_form();
	}
	for (int i = 0; i < form_height; i++)
	{
		for (int j = 0; j < form_width; j++)
		{
			if (tetris[position_y + i][position_x + j] != ' ')
			{
				dr = 0;
				return;
			}
		}
	}
}

void next_position_check()
{
	if (dr != 0)
	{
		rotation_number += dr;
		rotation_form();
	}
	else
	{
		for (int i = 0; i < form_height; i++)
		{
			for (int j = 0; j < form_width; j++)
			{
				// dx, dy, dr은 개별 처리

				/*

				 회전시 안되는 경우
				1. 만약 dr이 1일때, 현재위치에서의 rotation_number를 +1 해준상태가 벽을 침범할때.

				 조건 1. if(dr == 1) or if(dr != 0)
					--> dr이 1일때

				 조건 2. if(tetris_block[form_number][rotation_number + dr][i][j] == '@')
					--> rotation_number + 1이 된 상태에서 @가 존재하는(실질적인 블록 형태를 띄는) 칸에서부터

				 조건 3. if(tetris[position_y + i][position_x + j] != ' ')
					--> 다음형태가 들어설 곳에 공백이 없다면
				 결과 : dr = 0;

				*/

				if (tetris_block[form_number][rotation_number][i][j] == '@')
				{
					if (dx != 0)
					{
						if (tetris[position_y + i][position_x + j + dx] != ' ')
						{
							dx = 0;
						}
					}
					if (dy != 0)
					{
						if (tetris[position_y + i + dy][position_x + j] == ' ')
						{
							dy = 1;
						}
						else
						{
							dy = -1;
							return;
						}
					}
				}
			}
		}
	}
}


// 좌표에 넣어주는 함수
void input_position()
{
	for (int i = 0; i < form_height; i++)
	{
		for (int j = 0; j < form_width; j++)
		{
			// 
			if (tetris_block[form_number][rotation_number][i][j] == '@')
			{
				tetris[position_y + i][position_x + j] = '@';
			}
		}
	}
}

// 이전상태를 지워주는 함수
// 이전상태와 바뀐상태를 비교해야 한다. 
void input_void()
{
	for (int i = 0; i <= form_height; i++)
	{ //  원래 있던 자리에 빈칸으로 채우기
		for (int j = 0; j <= form_width; j++)
		{
			if (tetris_block[form_number][rotation_number][i][j] == '@')
			{ 
				putsxy(default_x + position_x + j, default_y + position_y + i, " ");

			}
		}
	}
}

// 바뀐상태를 보여주는 함수
void show_move()
{
	for (int i = 0; i < form_height; i++)
	{   // 움직일 자리로 움직임
		for (int j = 0; j < form_width; j++)
		{
			if (tetris_block[form_number][rotation_number][i][j] == '@')
			{
				putsxy(default_x + position_x + j, default_y + position_y + i, "@");
			}
		}
	}
}


int main()
{
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
		// 키보드가 눌리는지 아닌지 판단.
		if (_kbhit())  // dx 입력 부분
		{
			input_keyboard();
		}
		else
		{
			dy = 1;
		}


		// dy, dx 결정 부분
		next_position_check();

		if (dy == -1)
		{   // 테트리스 좌표에 직접 넣기
			input_position();

			dy = 0;
			position_y = 0;
			position_x = 4;

			make_block();
		}

		if (dx != 0 || dy != 0 || dr != 0)
		{
			// 공백 삽입
			input_void();

			// 다음 위치
			position_x = position_x + dx;
			position_y = position_y + dy;

			// 보여주기
			show_move(); 
		}
		delay(100);
	}
	return 0;
}

// 변화되는 것들
// dx가 변하는것은 좌우가 움직이는것
// dy가 움직이는것은 count, 시간에 따라서 움직이는것
// rotation이 변하는것은 키보드를 입력받을때 움직임 => dx가 변하는것과 동일하게 움직인다.
// rotation에 관하여...
// 이전상태  -> 지우기 ->  바뀐상태
// 바뀐상태와 이전상태가 같으면 지우지 않아도 된다
// +) rotation 변화까지 생각

// 이제는 회전시킬때, 변하면 안되는 경우가 있을수 있으니까 그것을 마무리 하고난다음에...
// 한줄 다 채우면 없어지는것을 만들어야지