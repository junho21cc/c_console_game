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
    clock_t CurTime, OldTime;
    ScreenInit();
    Init();//�ʱ�ȭ

    while (1)
    {
        int nKey = GetKeyEvent();
        if (nKey == 'q')
            break;
        KeyProcess(nKey);
        Update();//������ ����
        Render();//ȭ�� ���
        WaitRender(clock());
    }
    Release();//����
    ScreenRelease();
    return 0;
}


// ��ó: https://huiyu.tistory.com/entry/C����-���Ӹ����-3-������-���-����?category=463903 [huiyu's blog]