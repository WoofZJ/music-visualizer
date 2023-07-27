#ifndef FFT_H
#define FFT_H

#include <complex.h>
#include <stdlib.h>

extern const float pi;

void dft(float in[], float complex out[], size_t n);
void fft(float in[], size_t stride, float complex out[], size_t n);

#endif