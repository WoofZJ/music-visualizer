#include "fft.h"
#include <complex.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

const float pi = 3.1415926f;

void dft(float in[], float complex out[], size_t n) {
    for (size_t f = 0; f < n; ++f) {
        out[f] = 0;
        for (size_t i = 0; i < n; ++i) {
            float t = (float)i/n;
            out[f] += in[i]*cexpf(2*I*pi*t*f);
        }
    }
}

void fft(float in[], size_t stride, float complex out[], size_t n) {
    
    assert(n > 0);
    if (n == 1) {
        out[0] = in[0];
        return;
    }

    fft(in, stride*2, out, n/2);
    fft(in+stride, stride*2, out+n/2, n/2);

    for (size_t k = 0; k < n/2; ++k) {
        float t = (float)k/n;
        float complex v = cexp(-2*I*pi*t)*out[k+n/2];
        float complex e = out[k];
        out[k] = e+v;
        out[k+n/2] = e-v;
    }
}