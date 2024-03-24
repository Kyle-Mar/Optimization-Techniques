#include <iostream>
#include "Wave.h"
#include "Stopwatch.h"
#include "ymm.h"
#include "xmm.h"
#include <immintrin.h>

using namespace std;/*

int main(int argc, char* argv[])
{
    string filename = argv[1];

    Wave inwave(filename);
    if (inwave.format.format != 1 || inwave.format.numChannels != 1 || inwave.format.bitsPerSample != 8) {
        cout << "Not an 8 bit mono wave\n";
        return 1;
    }

    int nf = inwave.numFrames;
    Wave outwave(inwave.format, inwave.numFrames/2);
    int32_t* ip = (int32_t*)inwave.data();
    int32_t* op = (int32_t*)outwave.data();
    ymmi8 mask(14, 12, 10, 8, 6, 4, 2, 0, 15, 13, 11, 9, 7, 5, 3, 1, 14, 12, 10, 8, 6, 4, 2, 0, 15, 13, 11, 9, 7, 5, 3, 1);
    Stopwatch swatch;
    swatch.start();
    for (unsigned i = 0; i < nf; i+=16, ip+=4, op+=2) {
        ymmi32 in(ip);
        ymmi32 shuffled = in.shuffle(mask);
        ymmi32 permuted = _mm256_permute4x64_epi64(shuffled, 0b11011000);
        permuted.toxmm().store(op);
    }
    swatch.stop();
    std::cout << swatch.elapsed_us() << " elapsed us" << std::endl;
    outwave.write("out.wav");

    return 0;
}*/
