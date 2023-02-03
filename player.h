#define GROUND 50

typedef struct PLAYER {
  VECTOR  pos;
  SVECTOR rot;
  int     jumping;
  int     N, S, E, W;
  int     speedY;
} PLAYER;

extern PLAYER player;