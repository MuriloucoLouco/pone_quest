#include "global.h"
#include "system.h"
#include "model.h"
#include "texture.h"
#include "files.h"

MATRIX light_mtx = {
	-1024, -1024, -1024,
	0	   , 0	  , 0,
	0	   , 0	  , 0
};

MODELS models;

void initModels() {
	models.apple = (MODEL*)file2model("APPLE.MDL", textures.apple);
	models.ground = (MODEL*)file2model("GROUND.MDL", textures.ground);
	models.pony = (MODEL*)file2model("TWILY.MDL", textures.pony);
	models.cube = (MODEL*)file2model("CUBE.MDL", textures.apple);
	models.shadow = (MODEL*)file2model("SHADOW.MDL", textures.shadow);
	models.casa1 = (MODEL*)file2model("CASA1.MDL", textures.casa1);
}

void addModel(MODEL *model, char **primaddr, VECTOR pos_, SVECTOR rot_) {
	int i, depth;
	MATRIX mtx, lmtx;

	VECTOR pos = {pos_.vx>>12, pos_.vy>>12, pos_.vz>>12};
	SVECTOR rot = rot_;

	RotMatrix(&rot, &mtx);
	TransMatrix(&mtx, &pos);
	MulMatrix0(&light_mtx, &mtx, &lmtx);

	gte_SetRotMatrix(&mtx);
	gte_SetTransMatrix(&mtx);
	gte_SetLightMatrix(&lmtx);

	POLY_FT4 *pol4;
	POLY_FT3 *pol3;

	u_short tpage = getTPage((int)model->texture->pmode&0x3, 0, model->texture->px, model->texture->py);

	for (i = 0; i < model->face_number; i++) {
		if (model->indices->m == 1) { //quad
			pol4 = (POLY_FT4*)*primaddr;

			gte_ldv3(
				&model->verts[model->indices[i].v0],
				&model->verts[model->indices[i].v1],
				&model->verts[model->indices[i].v2]
			);
			gte_rtpt();
			gte_nclip();
			gte_stopz(&depth);
			if (depth < 0) continue;

			gte_avsz4();
			gte_stotz(&depth);
			if ((depth >> 2) <= 0 || (depth >> 2) >= OTLEN) continue;

			setPolyFT4(pol4);

			gte_stsxy0(&pol4->x0);
			gte_stsxy1(&pol4->x1);
			gte_stsxy2(&pol4->x2);

			gte_ldv0(&model->verts[model->indices[i].v3]);
			gte_rtps();
			gte_stsxy(&pol4->x3);

			gte_ldrgb(&pol4->r0);
			gte_ldv0(&model->normals[model->vert_number + i]);
			gte_ncs();
			gte_strgb(&pol4->r0);

			pol4->tpage = tpage;
			setUV4(pol4,
				model->indices[i].t0x, model->indices[i].t0y,
				model->indices[i].t1x, model->indices[i].t1y,
				model->indices[i].t2x, model->indices[i].t2y,
        model->indices[i].t3x, model->indices[i].t3y
      );
			if (model->texture->pmode & 0x8) {
				if (depth < 100) {
					setClut(pol4, model->texture->cx, model->texture->cy);
				} else if (((depth-100)>>5) < 8) {
					setClut(pol4, model->texture->cx, model->texture->cy+((depth-100)>>5));
				} else {
					setClut(pol4, model->texture->cx, model->texture->cy+8);
				}
			}
			setShadeTex(pol4, 1);
			addPrim(ot[db]+(depth>>2), pol4);
			*primaddr += sizeof(POLY_FT4);
		} else { //tri
			pol3 = (POLY_FT3*)*primaddr;

			gte_ldv3(
				&model->verts[model->indices[i].v0],
				&model->verts[model->indices[i].v1],
				&model->verts[model->indices[i].v2]
			);
			gte_rtpt();
			gte_nclip();
			gte_stopz(&depth);
			if (depth < 0) continue;

			gte_avsz3();
			gte_stotz(&depth);
			if ((depth >> 2) <= 0 || (depth >> 2) >= OTLEN) continue;

			setPolyFT3(pol3);

			gte_stsxy0(&pol3->x0);
			gte_stsxy1(&pol3->x1);
			gte_stsxy2(&pol3->x2);

			pol3->tpage = tpage;
			setUV3(pol3, 
				model->indices[i].t0x, model->indices[i].t0y,
        model->indices[i].t1x, model->indices[i].t1y,
        model->indices[i].t2x, model->indices[i].t2y
      );
			if (model->texture->pmode & 0x8) {
				if (depth < 100) {
					setClut(pol3, model->texture->cx, model->texture->cy);
				} else if (((depth-100)>>5) < 8) {
					setClut(pol3, model->texture->cx, model->texture->cy+((depth-100)>>5));
				} else {
					setClut(pol3, model->texture->cx, model->texture->cy+8);
				}
			}

			gte_ldrgb(&pol3->r0);
			gte_ldv0(&model->normals[model->vert_number + i]);
			gte_ncs();
			gte_strgb(&pol3->r0);
			setShadeTex(pol3, 1);
			addPrim(ot[db]+(depth>>2), pol3);
			*primaddr += sizeof(POLY_FT3);
		}
	}
}

void addSprite(GsIMAGE *texture, char **primaddr, VECTOR pos_) {
	POLY_FT4 *pol4 = (POLY_FT4*)*primaddr;
	VECTOR pos = {pos_.vx>>12, pos_.vy>>12, pos_.vz>>12};
	SVECTOR rot = {0, 0, 0, 0};
	MATRIX mtx, lmtx;
	int depth;

	RotMatrix(&rot, &mtx);
	TransMatrix(&mtx, &pos);
	MulMatrix0(&light_mtx, &mtx, &lmtx);
	gte_SetRotMatrix(&mtx);
	gte_SetTransMatrix(&mtx);
	gte_SetLightMatrix(&lmtx);
	
	SVECTOR v0 = {-2*texture->pw>>1, -texture->ph>>1, 0, 0};
	SVECTOR v1 = {+2*texture->pw>>1, -texture->ph>>1, 0, 0};
	SVECTOR v2 = {-2*texture->pw>>1, +texture->ph>>1, 0, 0};
	SVECTOR v3 = {+2*texture->pw>>1, +texture->ph>>1, 0, 0};

	gte_ldv3(&v0, &v1, &v2);
	gte_rtpt();
	gte_nclip();
	gte_stopz(&depth);

	gte_avsz3();
	gte_stotz(&depth);

	setPolyFT4(pol4);

	gte_stsxy0(&pol4->x0);
	gte_stsxy1(&pol4->x1);
	gte_stsxy2(&pol4->x2);

	gte_ldv0(&v3);
	gte_rtps();
	gte_stsxy(&pol4->x3);

	pol4->tpage = getTPage((int)texture->pmode&0x3, 0, texture->px, texture->py);
	setUV4(pol4, 0, 0,
		2*texture->pw, 0,
		0, texture->ph,
		2*texture->pw, texture->ph
	);
	
	if (texture->pmode & 0x8) {
		if (depth < 100) {
			setClut(pol4, texture->cx, texture->cy);
		} else {
			setClut(pol4, texture->cx, texture->cy+((depth-100)>>5));
		}
	}

	setShadeTex(pol4, 1);

	addPrim(ot[db]+(depth>>2), pol4);
	*primaddr += sizeof(POLY_FT4);
}