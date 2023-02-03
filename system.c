#include "global.h";
#include "system.h";

int db;
u_int ot[2][OTLEN];
char pribuff[2][32768];
char *nextpri;
DISPENV disp[2];
DRAWENV draw[2];
u_char padbuff[2][34];
PADTYPE *pad;

MATRIX color_mtx = {
	ONE, 0, 0,
	ONE, 0, 0,
	ONE, 0, 0	
};

void init() {
	ResetGraph(0);

	CdInit();
	
	SetDefDispEnv(&disp[0], 0, 0, SCREEN_XRES, SCREEN_YRES);
	SetDefDispEnv(&disp[1], 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);

	SetDefDrawEnv(&draw[0], 0, SCREEN_YRES, SCREEN_XRES, SCREEN_YRES);
	SetDefDrawEnv(&draw[1], 0, 0, SCREEN_XRES, SCREEN_YRES);

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

	initTextures();
	initModels();
	initObjects();
	initMap();
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

  ClearOTagR(ot[db], OTLEN);
}