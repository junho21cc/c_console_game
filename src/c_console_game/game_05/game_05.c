#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>
#include <windows.h>
#include "Screen.h"
#include "Fps.h"
#include "Missile.h"

FPSData* fpsData;
int hero_x, hero_y;

Missile m;

void Init()
{
    InitFPSData(&fpsData);
    hero_x = 10;
    hero_y = 10;

    MissileDestroy(&m);
}

void Update()
{
    clock_t CurTime = clock();
    
    if (CurTime - m.move_time > m.velocity)
    {
        m.y--;
        m.move_time = CurTime;

        if (m.y < -1)
        {
            MissileDestroy(&m);
        }
    }
}

void Render()
{
    ScreenClear();
    DrawFPS(&fpsData);

    ScreenPrint(hero_x, hero_y, "A");

    if (m.x >= 0 && m.y >= 0)
    {
        ScreenPrint(m.x, m.y, "^");
    }

    ScreenFlipping();
}

void Release()
{
    DestoyFPSData(&fpsData);
}

void WaitRender(clock_t OldTime)
{
    int CurTime;
    
    while (1)
    {
        CurTime = clock();
        if (CurTime - OldTime > 1)
        {
            break;
        }
    }
}

int GetKeyEvent()
{
    if (_kbhit())
    {
        return _getch();
    }
    return 0;
}

int KeyProcess(int key)
{
    if (key == 'q')
        return 1;
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
    case 'a':
        m = MissileCreate(hero_x, hero_y - 1, 200);
        break;
    }
    return 0;
}

int main()
{
    int nKey;
    clock_t CurTime, OldTime;
    ScreenInit();
    Init();//초기화
    

    while (1)
    {
        OldTime = clock();

        nKey = GetKeyEvent();
        if (KeyProcess(nKey) == 1)
        {
            break;
        }
               
        Update();//데이터 갱신
        Render();//화면 출력

        WaitRender(OldTime);
        
    }
    Release();//해제
    ScreenRelease();
    return 0;
}


// 출처: https://huiyu.tistory.com/entry/C언어로-게임만들기-3-렌더링-기능-구현?category=463903 [huiyu's blog]