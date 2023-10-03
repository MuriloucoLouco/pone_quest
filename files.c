#include "global.h"
#include "model.h"
#include "texture.h"
#include "files.h"

char *loadfile(const char *filename) {
  CdlFILE filePos;
  int numsecs;
  unsigned int *buff;

  buff = NULL;

  if (!CdSearchFile(&filePos, filename)) {
    printf("ERROR READING FILE: ");
    printf(filename);
  } else {
    numsecs = (filePos.size+2047)/2048;
    buff = (unsigned int*)malloc(sizeof(unsigned) + 2048 * numsecs);

    CdControl(CdlSetloc, (unsigned char*)&filePos.pos, 0);
    CdRead(numsecs, buff, CdlModeSpeed);
    CdReadSync(0, 0);
    buff[(2048 / sizeof(unsigned)) * numsecs] = NULL;
  }

  return buff;
}

int pow(int n, int p) {
  u_int i;
  int r = 1;
  for (i = 0; i < p; i++) {
    r *= n;
  }
  return r;
}

int extract_digit(char *buffer, unsigned int *ind) {
  u_int j, d = 0;
  int value = 0;
  int negative = 1;
  char digit_buffer[8];

  while (buffer[*ind] == ' ' || buffer[*ind] == '\n') {
    (*ind) = (*ind) + 1;
  }

  while (buffer[*ind] != NULL && buffer[*ind] != ' ' && buffer[*ind] != '\n') {
    if (buffer[*ind] == '-') {
      negative = -1;
    } else {
      digit_buffer[d] = buffer[*ind];
      d++;
    }
    (*ind) = (*ind) + 1;
  }

  for (j = 0; j < d; j++) {
    value += ((int)digit_buffer[d-j-1] - 48) * pow(10, j);
  }

  return negative * value;
}

MODEL *file2model(const char *filename, GsIMAGE *model_texture) {
  int vert_number, normal_number, face_number;
  u_int i = 0;
  u_int k = 0;
  int current_digit = 0;

  char *buffer = loadfile(filename);

  if (buffer == NULL) {
    printf("ERROR CONVERTING FILE: ");
    printf(filename);
    return NULL;
  }

  vert_number = extract_digit(buffer, &i);
  normal_number = extract_digit(buffer, &i);
  face_number = extract_digit(buffer, &i);

  MODEL *model = malloc(sizeof(*model));

  model->vert_number = vert_number;
  model->normal_number = normal_number;
  model->face_number = face_number;
  model->texture = model_texture;
  model->verts = malloc(vert_number * sizeof(SVECTOR));
  model->normals = malloc(normal_number * sizeof(SVECTOR));
  model->indices = malloc(face_number * sizeof(INDEX));

  for (k = 0; k < vert_number; k++) {
    model->verts[k].vx = extract_digit(buffer, &i);
    model->verts[k].vy = extract_digit(buffer, &i);
    model->verts[k].vz = extract_digit(buffer, &i);
  }
  
  for (k = 0; k < normal_number; k++) {
    model->normals[k].vx = extract_digit(buffer, &i);
    model->normals[k].vy = extract_digit(buffer, &i);
    model->normals[k].vz = extract_digit(buffer, &i);
  }

  for (k = 0; k < face_number; k++) {
    model->indices[k].m = extract_digit(buffer, &i);
    model->indices[k].v0 = extract_digit(buffer, &i);
    model->indices[k].v1 = extract_digit(buffer, &i);
    model->indices[k].v2 = extract_digit(buffer, &i);
    model->indices[k].v3 = extract_digit(buffer, &i);
    model->indices[k].t0x = extract_digit(buffer, &i);
    model->indices[k].t0y = extract_digit(buffer, &i);
    model->indices[k].t1x = extract_digit(buffer, &i);
    model->indices[k].t1y = extract_digit(buffer, &i);
    model->indices[k].t2x = extract_digit(buffer, &i);
    model->indices[k].t2y = extract_digit(buffer, &i);
    model->indices[k].t3x = extract_digit(buffer, &i);
    model->indices[k].t3y = extract_digit(buffer, &i);
  }
  
  free(buffer);
  return model;
}

GsIMAGE *file2texture(const char *filename) {
  u_int *tim_data = loadfile(filename);
  GsIMAGE *texture = malloc(sizeof(GsIMAGE));
  LoadTexture(tim_data, texture);
  free(tim_data);
    
  return texture;
}