#include <malloc.h>
#include "MissileList.h"


void MissileListInsert(MissileList* list, int x, int y, int velocity)
{
	MissileItem* m = malloc(sizeof(MissileItem));
	m->x = x;
	m->y = y;
	m->velocity = velocity;
	m->move_time = clock();
	m->next = list->head;

	list->head = m;
}