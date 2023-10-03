#include "global.h"
#include "system.h"
#include "model.h"
#include "player.h"
#include "level.h"

int main() {
	int n = 0;

	init();

	while (1) {
		move(&player);
		moveMap();

		addModel(models.pony, &nextpri, player.pos, player.rot);
		displayMap();

		FntPrint(-1, "N:%d S:%d E:%d W:%d", player.N, player.S, player.E, player.W);
		FntFlush(-1);

		display();
		n++;
	}

	return 0;
}
