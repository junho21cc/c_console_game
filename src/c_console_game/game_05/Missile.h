#pragma once
#include <time.h>

typedef struct Missile
{
    int velocity;   // 단위: ms,  1칸을 움직이는데 걸리는 시간 (예: 200)
    int x, y;       // 현재 위치
    clock_t move_time; // 최근에 움직인 시각

} Missile;

Missile MissileCreate(int x, int y, int velocity);
void MissileDestroy(Missile * m);
int MissileExist(Missile* m);

