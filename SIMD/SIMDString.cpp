#include "Image.h"
#include <iostream>
#include <cmath>
#include <cassert>
#include "Stopwatch.h"
#include "xmm.h"
#include <smmintrin.h>

//for debugging...
#ifndef VERBOSE
#define VERBOSE 0
#endif


/*int main(int argc, char* argv[])
{

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " imageFile\n";
        return 1;
    }

    std::string filename = argv[1];
    Image i(filename);

    if (i.format() != "RGBA8") {
        std::cout << "Unsupported image format\n";
        return 1;
    }

    Stopwatch swatch;
    swatch.start();

    unsigned w = i.width();
    unsigned h = i.height();
    unsigned num_bytes = w * h * 4;
    uint8_t* pix = i.pixels();
    char range1[] = {0,63, 0};
    char range2[] = {64, 127, 0};
    char range3[] = {128, 191, 0};
    char range4[] = {192, 255, 0 };
    __m128i range1_128i = _mm_loadu_si128((__m128i*) range1);
    __m128i range2_128i = _mm_loadu_si128((__m128i*) range2);
    __m128i range3_128i = _mm_loadu_si128((__m128i*) range3);
    __m128i range4_128i = _mm_loadu_si128((__m128i*) range4);
    for (int i = 0; i < num_bytes; i+=16, pix+=16) {

        __m128i pixRow = _mm_loadu_si128((__m128i*) pix);
        __m128i res = _mm_cmpestrm(range1_128i, 2, pixRow, 16, _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES | _SIDD_UNIT_MASK);
        __m128i a = _mm_set1_epi8(0);
        __m128i v = _mm_blendv_epi8(pixRow, a, res);
        
        __m128i res2 = _mm_cmpestrm(range2_128i, 2, pixRow, 16, _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES | _SIDD_UNIT_MASK);
        __m128i a2 = _mm_set1_epi8(96);
        __m128i v2 = _mm_blendv_epi8(v, a2, res2);

        __m128i res3 = _mm_cmpestrm(range3_128i, 2, pixRow, 16, _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES | _SIDD_UNIT_MASK);
        __m128i a3 = _mm_set1_epi8(172);
        __m128i v3 = _mm_blendv_epi8(v2, a3, res3);

        __m128i res4 = _mm_cmpestrm(range4_128i, 2, pixRow, 16, _SIDD_UBYTE_OPS | _SIDD_CMP_RANGES | _SIDD_UNIT_MASK);
        __m128i a4 = _mm_set1_epi8(255);
        __m128i v4 = _mm_blendv_epi8(v3, a4, res4);
        _mm_storeu_si128((__m128i*) pix, v4);

    }

    swatch.stop();

    i.writePng("out.png");
    std::cout << "Elapsed us: " << swatch.elapsed_us() << "usec" << std::endl;

    return 0;
}*/