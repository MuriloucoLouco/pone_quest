#include "global.h"
#include "main.h"

typedef struct PLAYER {
  VECTOR  pos;
  SVECTOR rot;
  int     jumping;
  int     N, S, E, W;
  int     speedY;
} PLAYER;

PLAYER player = {
  {0, -ONE * GROUND, 200 * ONE},
  {0, 1024, 0, 0},
  1
};

int speed = 3;

void rotate(PLAYER *pl, int angle) {
  int dtheta = angle - pl->rot.vy;
  int negative = 1;
  if (abs(dtheta) > 2048) negative = -1;
  if (dtheta > 0) {
    pl->rot.vy += negative*128;
  }
  if (dtheta < 0) {
    pl->rot.vy -= negative*128;
  }
}

void move(PLAYER *pl) {
  pad = (PADTYPE*)padbuff[0];

  pl->N = 0; pl->S = 0; pl->E = 0; pl->W = 0;
  if (pad->stat == 0 && (pad->type == 0x4 || pad->type == 0x5 || pad->type == 0x7)) {
    if (!(pad->btn & PAD_UP)) {
      pl->N = 1;
      pl->S = 0;
    }
    if (!(pad->btn & PAD_DOWN)) {
      pl->S = 1;
      pl->N = 0;
    }
    if (!(pad->btn & PAD_RIGHT)) {
      pl->E = 1;
      pl->W = 0;
    }
    if (!(pad->btn & PAD_LEFT)) {
      pl->W = 1;
      pl->E = 0;
    }
    if (!(pad->btn & PAD_CROSS) && !pl->jumping) {
      pl->speedY = -3 * ONE;
      pl->jumping = 1;
    }
  }
  if (pl->S && !(pl->W || pl->E)) {rotate(pl, 3072);    pl->pos.vz -= speed * ONE;}
  if (pl->S && pl->W) 				    {rotate(pl, 3584);  pl->pos.vz -= speed * 2896; pl->pos.vx -= speed * 2896;}
  if (pl->W && !(pl->S || pl->N)) {rotate(pl, 0); pl->pos.vx -= speed * ONE;}
  if (pl->W && pl->N) 			      {rotate(pl, 512); pl->pos.vz += speed * 2896; pl->pos.vx -= speed * 2896;}
  if (pl->N && !(pl->W || pl->E)) {rotate(pl, 1024); pl->pos.vz += speed * ONE;}
  if (pl->N && pl->E) 		  	  	{rotate(pl, 1536); pl->pos.vz += speed * 2896; pl->pos.vx += speed * 2896;}
  if (pl->E && !(pl->N || pl->S)) {rotate(pl, 2048); pl->pos.vx += speed * ONE;}
  if (pl->E && pl->S) 				    {rotate(pl, 2560); pl->pos.vz -= speed * 2896; pl->pos.vx += speed * 2896;}

  if (pl->jumping) {
    pl->speedY += 512;
  }
  pl->pos.vy += pl->speedY;
  
  if (pl->rot.vy > 4096) pl->rot.vy = 0;
  if (pl->rot.vy < 0) pl->rot.vy = 4096;
  if (pl->pos.vy > ONE * GROUND) {
    pl->pos.vy = ONE * GROUND;
    pl->speedY = 0;
    pl->jumping = 0;
  }
  if (pl->pos.vx < -100 * ONE) pl->pos.vx = -100 * ONE;
  if (pl->pos.vx > 100 * ONE) pl->pos.vx = 100 * ONE;
  if (pl->pos.vz < 100 * ONE) pl->pos.vz = 100 * ONE;
}