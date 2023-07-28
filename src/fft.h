#ifndef FFT_H
#define FFT_H

#include <complex.h>
#include <stdlib.h>

extern const float pi;

#ifdef _MSC_VER

void dft(float in[], _Fcomplex out[], size_t n);
void fft(float in[], size_t stride, _Fcomplex out[], size_t n);

#else 

void dft(float in[], float complex out[], size_t n);
void fft(float in[], size_t stride, float complex out[], size_t n);

#endif

#endif