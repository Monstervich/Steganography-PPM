#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PPM1 'P'
#define PPM2 '3'
#define PPM_FORMAT (PPM1 * 255 + PPM2)
#define PPM_TYPE PPM_FORMAT

#define PPM_MAXVAL 255
#define PPM_MAXMESS 100 //defines max for encoding and decoding
#define BMAX 100 //defines max for buffer

/* Typedef struct holds integer rgb values within type identifier pixel */
typedef struct pixel{
	int r, g, b;
} pixel;

/* text struct holds strings of comments using LLDS */
typedef struct text {
	char * text;
	struct text * next;
}text;

/* PART A - Struct created to hold PPM Images(Uses a 2D array for rows and cols) */
struct PPM {

	char * type;
	struct text * text;
	int cn;
	int w, h, max; //Width, height and max size
	struct pixel ** p;

};


/* Declaration of functions. Implementations made in ppmfun.c */

char * readLine(FILE * fin);

void showPPM(struct PPM * i);

void getText(FILE * fin, struct PPM * i);

void getPixels(FILE * fin, struct PPM * i);

struct PPM * getPPM(FILE * fin);

struct PPM * encode (char * txt, struct PPM * i);

void writePPM(FILE * fin ,struct PPM * i, const char *fileName);

char * decode(struct PPM * i1, struct PPM * i2);

void encodeFile(int argc, char const ** argv);

void decodeFile(int argc, char const ** argv);
