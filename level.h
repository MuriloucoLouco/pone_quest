#include "model.h"

#define OBJ_LIST_LEN 30

typedef struct OBJECT {
  MODEL *model;
  int   line;
  int   z;
} OBJECT;

extern OBJECT object_list[OBJ_LIST_LEN];