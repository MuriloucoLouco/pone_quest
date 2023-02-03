#include "global.h"
#include "system.h"
#include "model.h"
#include "player.h"

int main() {
	int n = 0;

	init();

	while (1) {
		move(&player);
		moveMap();

		addModel(models.pony, &nextpri, player.pos, player.rot);
		displayMap();

		FntPrint(-1, "TESTE");
		FntFlush(-1);

		display();
		n++;
	}

	return 0;
}