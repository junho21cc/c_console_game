
#include<stdio.h>

struct game
{
	int cur_block;
	int cur_block_rot;

};

void game_init(struct game * g)
{
	(*g).cur_block = 1;
	(*g).cur_block_rot = 2;

	g->cur_block = 1;
	g->cur_block_rot = 2;

}

int main()
{
	struct game g;

	game_init(&g);

	while (1) {
		process_internal(&g);
		process_input(&g);
		process_display(&g);
	}


	return 0;
}


