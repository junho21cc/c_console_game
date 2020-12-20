#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<malloc.h>
#include"Screen.h"

clock_t FPSCurTime, FPSOldTime;
int FrameCnt;
char* FPSTextBuffer;

void Init()
{
    FrameCnt = 0;
    FPSTextBuffer = (char*)malloc(sizeof(char) * 10);
    sprintf(FPSTextBuffer, "FPS:%d", FrameCnt);
    FPSOldTime = clock();
}

void Update()
{
}

void Render()
{
    ScreenClear();
    //출력코드
    FrameCnt++;
    FPSCurTime = clock();
    if (FPSCurTime - FPSOldTime >= 1000)
    {
        sprintf(FPSTextBuffer, "FPS:%d", FrameCnt);
        FPSOldTime = clock();
        FrameCnt = 0;
    }

    ScreenPrint(0, 0, FPSTextBuffer);

    //ScreenPrint(0, 0, FPSTextBuffer);
    ScreenFlipping();
}

void Release()
{
    free(FPSTextBuffer);
}

int main()
{
    int nKey;
    clock_t CurTime, OldTime;
    ScreenInit();
    Init();//초기화

    while (1)
    {
        if (_kbhit())
        {
            nKey = _getch();
            if (nKey == 'q')
                break;
            switch (nKey)
            {
            case 'j':
                break;
            case 'l':
                break;
            }
        }
        OldTime = clock();
        Update();//데이터 갱신
        Render();//화면 출력
        while (1)
        {
            CurTime = clock();
            if (CurTime - OldTime > 20)
                break;
        }
    }
    Release();//해제
    ScreenRelease();
    return 0;
}


// 출처: https://huiyu.tistory.com/entry/C언어로-게임만들기-3-렌더링-기능-구현?category=463903 [huiyu's blog]