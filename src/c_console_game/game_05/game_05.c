#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>
#include <windows.h>
#include "Screen.h"
#include "Fps.h"

FPSData* fpsData;
int hero_x, hero_y;

void Init()
{
    InitFPSData(&fpsData);
    hero_x = 10;
    hero_y = 10;
}

void Update()
{
}

void Render()
{
    ScreenClear();
    DrawFPS(&fpsData);

    ScreenPrint(hero_x, hero_y, "A");

    ScreenFlipping();
}

void Release()
{
    DestoyFPSData(&fpsData);
}

void WaitRender(clock_t OldTime)
{
    clock_t CurTime;
    while (1)
    {
        CurTime = clock();
        if (CurTime - OldTime > 33)
            break;
    }
}

int GetKeyEvent()
{
    if (_kbhit())
        return _getch();

    return -1;
}

int GetConsoleInput(INPUT_RECORD* pir)
{
    DWORD numRead;
    ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), pir, 1, &numRead);
    if (numRead > 0) {
        /*
        if (pir->EventType == KEY_EVENT)
            OutputDebugStringA("KEY_EVENT\n");
        else if (pir->EventType == MOUSE_EVENT)
            OutputDebugStringA("MOUSE_EVENT\n");
        */
        return 1;
    }
    return 0;
}

int InputProcess(INPUT_RECORD* pir)
{
    if (pir->EventType == KEY_EVENT && !pir->Event.KeyEvent.bKeyDown && pir->Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
        return 1;
    if (pir->EventType == MOUSE_EVENT /*&& pir->Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED*/) {
        hero_x = pir->Event.MouseEvent.dwMousePosition.X;
        hero_y = pir->Event.MouseEvent.dwMousePosition.Y;
    }
    return 0;
}

void KeyProcess(int key)
{
    switch (key)
    {
    case 'j':
        hero_x -= 1;
        break;
    case 'l':
        hero_x += 1;
        break;
    case 'k':
        hero_y += 1;
        break;
    case 'i':
        hero_y -= 1;
        break;
    }
}

int main()
{
    int nKey;
    INPUT_RECORD ir;
    clock_t CurTime, OldTime;
    ScreenInit();
    Init();//초기화

    while (1)
    {
        if (GetConsoleInput(&ir))
        {
            if (InputProcess(&ir))
                break;
        }

        /*
        int nKey = GetKeyEvent();
        if (nKey == 'q')
            break;
        KeyProcess(nKey);
        */
        Update();//데이터 갱신
        Render();//화면 출력
        WaitRender(clock());
    }
    Release();//해제
    ScreenRelease();
    return 0;
}


// 출처: https://huiyu.tistory.com/entry/C언어로-게임만들기-3-렌더링-기능-구현?category=463903 [huiyu's blog]