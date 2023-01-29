#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>
#include <psxpad.h>
#include <psxcd.h>
#include <inline_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct INDEX {
	int m, v0, v1, v2, v3, t0x, t0y, t1x, t1y, t2x, t2y, t3x, t3y;
} INDEX;

typedef struct MODEL {
  int       vert_number;
  int       normal_number;
  int       face_number;
  GsIMAGE   *texture;
  SVECTOR   *verts;
  SVECTOR   *normals;
  INDEX     *indices;
} MODEL;