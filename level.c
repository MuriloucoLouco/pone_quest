#include "global.h"
#include "level.h"
#include "system.h"
#include "player.h"
#include "texture.h"

#define GROUND_LENGHT 550*ONE

OBJECT object_list[OBJ_LIST_LEN];

int ground1;
int ground2;
int ground3;

int cloud1;
int cloud2;
int cloud3;
int cloud4;

void initMap() {
  ground1 = 100*ONE;
  ground2 = GROUND_LENGHT + ground1 - 6*ONE;
  ground3 = GROUND_LENGHT + ground2 - 6*ONE;

  cloud1 = 400*ONE;
  cloud2 = 800*ONE;
  cloud3 = 1200*ONE;
  cloud4 = 1600*ONE;
}

void moveMap() {
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
}

displayMap() {
  addModel(models.ground, &nextpri, (VECTOR){-100*ONE, GROUND*ONE+25*ONE, ground1}, (SVECTOR){0, 1024, 0, 0});
  addModel(models.ground, &nextpri, (VECTOR){-100*ONE, GROUND*ONE+25*ONE, ground2}, (SVECTOR){0, 1024, 0, 0});
  addModel(models.ground, &nextpri, (VECTOR){-100*ONE, GROUND*ONE+25*ONE, ground3}, (SVECTOR){0, 1024, 0, 0});
  addSprite(textures.cloud, &nextpri, (VECTOR){100*ONE, -5*GROUND*ONE, cloud1});
  addSprite(textures.cloud, &nextpri, (VECTOR){-200*ONE, -4*GROUND*ONE, cloud2});
  addSprite(textures.cloud, &nextpri, (VECTOR){400*ONE, -3*GROUND*ONE, cloud3});
  addSprite(textures.cloud, &nextpri, (VECTOR){-300*ONE, -4*GROUND*ONE, cloud4});
}

void initObjects() {
  int i;
  for (i = 0; i < OBJ_LIST_LEN; i++) {
    object_list[i].model = models.apple;
    object_list[i].line = 0;
    object_list[i].z = i+3;
  }
}

void placeObjects() {
  int i;
  for (i = 0; i < OBJ_LIST_LEN; i++) {
    VECTOR pos = {50 * object_list[i].line * ONE, ONE * GROUND, 50 * object_list[i].z * ONE};
    addModel(object_list[i].model, &nextpri, pos, (SVECTOR){0,0,0,0});
  }
}