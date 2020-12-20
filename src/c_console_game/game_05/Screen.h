#pragma once
void ScreenInit();
void ScreenFlipping();
void ScreenClear();
void ScreenRelease();
void ScreenPrint(int x, int y, char* string);
void SetColor(unsigned short color);


// 출처: https://huiyu.tistory.com/entry/C언어로-게임만들기-3-렌더링-기능-구현?category=463903 [huiyu's blog]