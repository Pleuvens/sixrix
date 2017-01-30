/*
 * FFTTest.h
 *
 * C header for ARM FFT algorithm
 */

#include "../fft/FFT.h"
#ifndef FFT_H
#define FFT_H

typedef void fFFT(void *input, int N);

/* FFT test data type */
typedef struct {
  int N;        /* number of points in the FFT */
  complex *in;   /* complex FFT test input */
  complex *out;    /* complex FFT test output */
} FFTData;

typedef void (*FFTFunction)(complex *, int N);

typedef struct {
	FFTFunction FFTpoint;
	char *FuncName;
	int Direction;
} FFTTest;


#endif
