#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <malloc.h>
#include <windows.h>
#include "Screen.h"
#include "Fps.h"
#include "MissileList.h"

#define MAX_MISSILE 10

FPSData* fpsData;
int hero_x, hero_y;

MissileList missile_list;

void Init()
{
    InitFPSData(&fpsData);
    hero_x = 10;
    hero_y = 10;

    missile_list.head = NULL;
}

void Update()
{
    clock_t CurTime = clock();
    MissileItem* m = missile_list.head;
    while (m) 
    {
        if (CurTime - m->move_time > m->velocity)
        {
            m->y--;
            m->move_time = CurTime;

            if (m->y < 0)
            {
                //MissileDestroy(&m);
            }
        }
        m = m->next;
    }
}

void Render()
{
    ScreenClear();
    DrawFPS(&fpsData);

    ScreenPrint(hero_x, hero_y, "A");
    MissileItem* m = missile_list.head;
    while (m)
    {
        ScreenPrint(m->x, m->y, "!");
        m = m->next;
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
        MissileListInsert(&missile_list, hero_x, hero_y - 1, 100);
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