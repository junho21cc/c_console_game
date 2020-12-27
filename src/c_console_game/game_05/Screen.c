#include <windows.h>

 static int g_nScreenIndex;
 static HANDLE g_hScreen[2];
 static DWORD g_OldConsoleMode, g_ConsoleMode;

void ScreenInit()
{
    CONSOLE_CURSOR_INFO cci;

    //ȭ�� ���� 2���� �����.
    g_hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
        0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    g_hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,
        0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

    //Ŀ�� �����
    cci.dwSize = 1;
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(g_hScreen[0], &cci);
    SetConsoleCursorInfo(g_hScreen[1], &cci);

    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &g_OldConsoleMode);
    g_ConsoleMode = g_OldConsoleMode | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    g_ConsoleMode &= ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_INSERT_MODE;
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), g_ConsoleMode);
}

void ScreenFlipping()
{
    SetConsoleActiveScreenBuffer(g_hScreen[g_nScreenIndex]);
    g_nScreenIndex = !g_nScreenIndex;
}

void ScreenClear()
{
    COORD Coor = { 0,0 };
    DWORD dw;
    COORD size = GetLargestConsoleWindowSize(g_hScreen[g_nScreenIndex]);
       FillConsoleOutputCharacter(g_hScreen[g_nScreenIndex], ' ', size.X * size.Y, Coor, &dw);
}

void ScreenRelease()
{
    CloseHandle(g_hScreen[0]);
    CloseHandle(g_hScreen[1]);
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), g_OldConsoleMode);
}

void ScreenPrint(int x, int y, char* string)
{
    DWORD dw;
    COORD CursorPosition = { x, y };
    SetConsoleCursorPosition(g_hScreen[g_nScreenIndex], CursorPosition);
    WriteFile(g_hScreen[g_nScreenIndex], string, strlen(string), &dw, NULL);

}

void SetColor(unsigned short color)
{
    SetConsoleTextAttribute(g_hScreen[g_nScreenIndex], color);
}


// ��ó: https://huiyu.tistory.com/entry/C����-���Ӹ����-3-������-���-����?category=463903 [huiyu's blog]