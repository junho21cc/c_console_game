#pragma once
#include<malloc.h>
#include<stdio.h>
#include<time.h>

typedef struct FPSData
{
    clock_t fpsCurTime, fpsOldTime;
    int frameCnt;
    char* fpsTextBuffer;
}FPSData;


void InitFPSData(FPSData** fpsData);

void DrawFPS(FPSData** fpsData);

void DestoyFPSData(FPSData** fpsData);


// 출처: https://huiyu.tistory.com/entry/C언어로-게임만들기-4-코드정리전체코드 [huiyu's blog]