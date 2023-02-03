#include "global.h"
#include "texture.h"

TEXTURES textures;

void initTextures() {
	textures.apple = file2texture("APPLE.TIM");
	textures.pony = file2texture("TWILY.TIM");
	textures.cloud = file2texture("CLOUD.TIM");
	textures.ground = file2texture("CHAO.TIM");
}

void DpqColor(CVECTOR *orgc, int depth, CVECTOR *newc) {
  gte_lddp(depth);
  gte_ldrgb(orgc);
  gte_dpcs();
  gte_strgb(newc);
}

void sce_make_fog_clut(RECT *crect, u_short *clut, int num) {
	int i,j;
	int stp;
	int depth;

	u_short newclut[256];

	CVECTOR orgc, newc;
	RECT rect;

	rect.x = crect->x;
	rect.w = crect->w;
	rect.h = crect->h;

	for (i = 0; i < num; i++) {
		depth = i * ONE / num;

		for (j = 0; j < crect->w; j++) {
			if (clut[j] == 0) {
			  newclut[j] = clut[j];
			} else {
			orgc.r = (clut[j] & 0x1f) << 3;
			orgc.g = ((clut[j] >> 5) & 0x1f) << 3;
			orgc.b = ((clut[j] >> 10) & 0x1f) << 3;
			stp	   = clut[j] & 0x8000;
			DpqColor(&orgc, depth, &newc);
			newclut[j] = stp | (newc.r >> 3)
											 | (((u_int)(newc.g & 0xf8)) << 2)
											 | (((u_int)(newc.b & 0xf8)) << 7);
			}
		}
		rect.y = crect->y + i;
		LoadImage(&rect, (u_int *)newclut);
	}
}

void LoadTexture(u_int *tim, GsIMAGE *tparam) {
	GsGetTimInfo(tim, tparam);
	printf("PRECT: %d %d %d %d\n", tparam->px, tparam->py, tparam->pw, tparam->ph);

	RECT prect = {tparam->px, tparam->py, tparam->pw, tparam->ph};
	RECT crect = {tparam->cx, tparam->cy, tparam->cw, tparam->ch};
	LoadImage(&prect, tparam->pixel);
	DrawSync(0);

	if (tparam->pmode & 0x8) {
    sce_make_fog_clut(&crect, tparam->clut, 8);
		DrawSync(0);
	}
}