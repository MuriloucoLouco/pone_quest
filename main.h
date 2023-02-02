#define SCREEN_XRES		320
#define SCREEN_YRES		240
#define CENTERX				SCREEN_XRES >> 1
#define CENTERY				SCREEN_YRES >> 1

#define OTLEN 4096
#define GROUND 50

int GsGetTimInfo(const uint32_t *tim, GsIMAGE *info);

extern int db;
extern u_int ot[2][OTLEN];
extern char pribuff[2][32768];
extern char *nextpri;

extern MATRIX color_mtx;
extern MATRIX light_mtx;

extern u_char padbuff[2][34];
extern PADTYPE *pad;