#include "Missile.h"

Missile MissileCreate(int x, int y, int velocity)
{
	Missile m;
	m.x = x;
	m.y = y;
	m.velocity = velocity;
	m.move_time = clock();
	return m;
}

void MissileDestroy(Missile* m)
{
	m->x = -1;
	m->y = -1;
}

int MissileExist(Missile* m)
{
	if (m->x > -1 && m->y > -1)
	{
		return 1;
	}
	return 0;
}