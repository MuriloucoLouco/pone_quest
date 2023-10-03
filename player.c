#include "global.h"
#include "system.h"
#include "player.h"

PLAYER player = {
  {0, -ONE * GROUND, 150 * ONE},
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
  int MOV = 0;
  if (pad->stat == 0 && (pad->type == 0x4 || pad->type == 0x5 || pad->type == 0x7)) {
    int UP = !(pad->btn & PAD_UP);
    int DOWN = !(pad->btn & PAD_DOWN);
    int RIGHT = !(pad->btn & PAD_RIGHT);
    int LEFT = !(pad->btn & PAD_LEFT);
    MOV = (UP || DOWN || RIGHT || LEFT);

    if (MOV) {
      pl->N = 0; pl->S = 0; pl->E = 0; pl->W = 0;
      if (UP) {pl->N = 1;}
      if (DOWN) {pl->S = 1;}
      if (RIGHT) {pl->E = 1;}
      if (LEFT) {pl->W = 1;}
    }

    if (!(pad->btn & PAD_CROSS) && !pl->jumping) {
      pl->speedY = -3 * ONE;
      pl->jumping = 1;
    }
  }
  if (pl->S && !(pl->W || pl->E)) {rotate(pl, 3072); if(MOV) {pl->pos.vz -= speed * ONE;}}
  if (pl->S && pl->W) 				    {rotate(pl, 3584); if(MOV) {pl->pos.vz -= speed * 2896; pl->pos.vx -= speed * 2896;}}
  if (pl->W && !(pl->S || pl->N)) {rotate(pl, 0);    if(MOV) {pl->pos.vx -= speed * ONE;}}
  if (pl->W && pl->N) 			      {rotate(pl, 512);  if(MOV) {pl->pos.vz += speed * 2896; pl->pos.vx -= speed * 2896;}}
  if (pl->N && !(pl->W || pl->E)) {rotate(pl, 1024); if(MOV) {pl->pos.vz += speed * ONE;}}
  if (pl->N && pl->E) 		  	  	{rotate(pl, 1536); if(MOV) {pl->pos.vz += speed * 2896; pl->pos.vx += speed * 2896;}}
  if (pl->E && !(pl->N || pl->S)) {rotate(pl, 2048); if(MOV) {pl->pos.vx += speed * ONE;}}
  if (pl->E && pl->S) 				    {rotate(pl, 2560); if(MOV) {pl->pos.vz -= speed * 2896; pl->pos.vx += speed * 2896;}}

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
  if (pl->pos.vx < -75 * ONE) pl->pos.vx = -75 * ONE;
  if (pl->pos.vx > 75 * ONE) pl->pos.vx = 75 * ONE;
  if (pl->pos.vz < 75 * ONE) pl->pos.vz = 75 * ONE;
}