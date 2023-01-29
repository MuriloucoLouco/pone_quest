#include "global.h"
#include "main.h"
#include "player.h"

#define GROUND_LENGHT 550*ONE

DISPENV disp[2];
DRAWENV draw[2];
int db;
u_int ot[2][OTLEN];
char pribuff[2][32768];
char *nextpri;

MATRIX color_mtx = {
	ONE, 0, 0,
	ONE, 0, 0,
	ONE, 0, 0	
};

MATRIX light_mtx = {
	-1024, -1024, -1024,
	0	   , 0	  , 0,
	0	   , 0	  , 0
};

u_char padbuff[2][34];
PADTYPE *pad;

void init() {
	ResetGraph(0);

	CdInit();
	
	SetDefDispEnv(&disp[0], 0, 0, SCREEN_XRES, SCREEN_YRES);
	SetDefDispEnv(&disp[1], 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);

	SetDefDrawEnv(&draw[0], 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);
	SetDefDrawEnv(&draw[1], 0, 0, SCREEN_XRES, SCREEN_YRES);
	//194, 245, 249
	setRGB0(&draw[0], 194, 245, 249);
	setRGB0(&draw[1], 194, 245, 249);
	draw[0].isbg = 1;
	draw[1].isbg = 1;
	draw[0].dtd = 1;
	draw[1].dtd = 1;

	PutDispEnv(&disp[0]);
	PutDrawEnv(&draw[0]);

	ClearOTagR(ot[0], OTLEN);
	ClearOTagR(ot[1], OTLEN);

	db = 0;
	nextpri = pribuff[db];

	InitGeom();
	gte_SetGeomOffset(CENTERX, CENTERY);
	gte_SetGeomScreen(CENTERX);

	gte_SetBackColor(64, 64, 64);
	gte_SetFarColor(194, 245, 249);
	gte_SetColorMatrix(&color_mtx);

	InitPAD(padbuff[0], 34, padbuff[1], 34);
	padbuff[0][0] = padbuff[0][1] = 0xff;
	padbuff[1][0] = padbuff[1][1] = 0xff;

	StartPAD();
	ChangeClearPAD(1);

	FntLoad(960, 0);
	FntOpen(0, 8, 320, 224, 0, 100);
}

void display() {
	DrawSync(0);
	VSync(0);

	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);

	SetDispMask(1);

	DrawOTag(ot[db]+OTLEN-1);

	db = !db;
	nextpri = pribuff[db];
}

int main() {
	int n = 0;

	int ground1 = 100*ONE;
	int ground2 = GROUND_LENGHT + ground1 - 6*ONE;
	int ground3 = GROUND_LENGHT + ground2 - 6*ONE;

	int cloud1 = 400*ONE;
	int cloud2 = 800*ONE;
	int cloud3 = 1200*ONE;
	int cloud4 = 1600*ONE;

	init();

	GsIMAGE *fur_texture = file2texture("FUR.TIM");
	GsIMAGE *chao_texture = file2texture("CHAO.TIM");
	GsIMAGE *cloud_texture = file2texture("CLOUD.TIM");
	MODEL *monkey = file2model("MONKEY.MDL", fur_texture);
	MODEL *ground = file2model("GROUND.MDL", chao_texture);

	while (1) {
		ground1 -= 3*ONE;
		ground2 -= 3*ONE;
		ground3 -= 3*ONE;
		cloud1 -=  3*ONE;
		cloud2 -=  3*ONE;
		cloud3 -=  3*ONE;
		cloud4 -=  3*ONE;
		if (ground2 <= 100*ONE) ground1 = ground3+GROUND_LENGHT;
		if (ground3 <= 100*ONE) ground2 = ground1+GROUND_LENGHT;
		if (ground1 <= 100*ONE) ground3 = ground2+GROUND_LENGHT;
		if (cloud1 < 0) cloud1 = 1600*ONE;
		if (cloud2 < 0) cloud2 = 1600*ONE;
		if (cloud3 < 0) cloud3 = 1600*ONE;
		if (cloud4 < 0) cloud4 = 1600*ONE;

		move(&player);

		ClearOTagR(ot[db], OTLEN);

		addModel(monkey, &nextpri, player.pos, player.rot);
		addModel(ground, &nextpri, (VECTOR){-100*ONE, GROUND*ONE+25*ONE, ground1}, (SVECTOR){0, 1024, 0, 0});
		addModel(ground, &nextpri, (VECTOR){-100*ONE, GROUND*ONE+25*ONE, ground2}, (SVECTOR){0, 1024, 0, 0});
		addModel(ground, &nextpri, (VECTOR){-100*ONE, GROUND*ONE+25*ONE, ground3}, (SVECTOR){0, 1024, 0, 0});
		addSprite(cloud_texture, &nextpri, (VECTOR){100*ONE, -5*GROUND*ONE, cloud1});
		addSprite(cloud_texture, &nextpri, (VECTOR){-200*ONE, -4*GROUND*ONE, cloud2});
		addSprite(cloud_texture, &nextpri, (VECTOR){400*ONE, -3*GROUND*ONE, cloud3});
		addSprite(cloud_texture, &nextpri, (VECTOR){-300*ONE, -4*GROUND*ONE, cloud4});

		FntPrint(-1, "%d %d %d %d", cloud1, cloud2, cloud3, cloud4);
		FntFlush(-1);

		display();
		n++;
	}

	return 0;
}