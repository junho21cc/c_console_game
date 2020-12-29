#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>
#include <windows.h>
#include "Screen.h"
#include "Fps.h"
#include "Missile.h"

#define MAX_MISSILE 10

FPSData* fpsData;
int hero_x, hero_y;

Missile m[MAX_MISSILE];

int FindEmptyMissileIndex()
{
    for (int i = 0; i < MAX_MISSILE; i++)
    {
        if (!MissileExist(&m[i]))
        {
            return i;
        }
    }
    return -1;
}

void Init()
{
    InitFPSData(&fpsData);
    hero_x = 10;
    hero_y = 10;
    for (int i = 0; i < MAX_MISSILE; i++)
    {
        MissileDestroy(&m[i]);
    }
}

void Update()
{
    clock_t CurTime = clock();
    
    for (int i = 0; i < MAX_MISSILE; i++)
    {
        if (MissileExist(&m[i]))
        {
            if (CurTime - m[i].move_time > m[i].velocity)
            {
                m[i].y--;
                m[i].move_time = CurTime;

                if (m[i].y < 0)
                {
                    MissileDestroy(&m[i]);
                }
            }
        }
    }
}

void Render()
{
    ScreenClear();
    DrawFPS(&fpsData);

    ScreenPrint(hero_x, hero_y, "A");

    for (int i = 0; i < MAX_MISSILE; i++)
    {
        if (MissileExist(&m[i]))
        {
            ScreenPrint(m[i].x, m[i].y, "!");
        }
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
    int empty_missile_index;

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
        empty_missile_index = FindEmptyMissileIndex();

        if (empty_missile_index != -1)
        {
            m[empty_missile_index] = MissileCreate(hero_x, hero_y - 1, 100);
        }
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