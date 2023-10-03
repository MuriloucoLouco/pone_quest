typedef struct TEXTURES {
  GsIMAGE *apple;
  GsIMAGE *pony;
  GsIMAGE *cloud;
  GsIMAGE *ground;
  GsIMAGE *shadow;
  GsIMAGE *casa1;
} TEXTURES;

extern TEXTURES textures;

void LoadTexture(u_int *tim, GsIMAGE *tparam);