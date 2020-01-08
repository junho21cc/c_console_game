#include <stdio.h>
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

int main()
{
	gotoxy(0, 4);
	for (int i = 0; i < 20; i++)
	{
		// 띄어쓰기 10번임
		printf("          %s\n", tetris[i]);
	}

	int random = rand() % 7;
	int ch;

	ch = getch();
	if (ch == 0xE0 || ch == 0)
	{
		ch = getch();

		for (int i = 0; i < 5; i++)
		{
			gotoxy(13, 4 + i);
			printf("%s\n", tetris_block[random][i]);

		}
	}

	for (;;)
	{
		for (int i = 0; i < 15; i++)
		{
			gotoxy(14, 8 + i);
			printf("%s\n", tetris_block[random][i]);
		}
	}

	return 0;
}