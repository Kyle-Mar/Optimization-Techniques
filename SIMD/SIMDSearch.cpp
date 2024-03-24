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
    GreyscaleImage imgA(argv[1]);
    GreyscaleImage imgB(argv[2]);
    int64_t chunkThreshold = atoi(argv[3]);     //if two chunks differ by this much: They are "not similar"

    //verify sizes match
    assert(imgA.width() == imgB.width());
    assert(imgA.height() == imgB.height());

    //shorten our code a bit


    //easier if we don't have to worry about leftovers
    assert(imgA.width() % 16 == 0);
    assert(imgA.height() % 16 == 0);
    int w = imgA.width();
    int h = imgA.height();
    Stopwatch sw;
    sw.start();
    xmmi64 totalDiff = xmmi64();
    xmmi64 grandTotal = xmmi64();
    //start of row in image A and image B
    uint8_t* rowA = imgA.pixels();
    uint8_t* rowB = imgB.pixels();

    //total number of chunks that don't match
    //cycle over each chunk; each chunk is 8x8
    for (int y = 0; y < h; y += 8) {

        //pointer to upper left corner of the current 8x8 chunk
        auto upperLeftCornerA = rowA;
        auto upperLeftCornerB = rowB;

        for (int x = 0; x < w; x += 16) {

#if VERBOSE
            std::cout << "Considering chunk starting at " << x << "," << y << "...\n";
#endif
            xmmi16 dif;
            for (int r = 0; r < 8; r++) {
                dif = dif + abs_dif(xmmu8(upperLeftCornerA + w * r), xmmu8(upperLeftCornerB + w * r));
                //totalDiff = totalDiff - xmmi64()
            }
            
            grandTotal = grandTotal - (xmmi64(dif) > xmmi64(chunkThreshold, chunkThreshold));
            
            upperLeftCornerA += 16;
            upperLeftCornerB += 16;

        }
        rowA += w * 8;
        rowB += w * 8;
    }

    sw.stop();
    std::int64_t numDissimilarChunks[2];
    grandTotal.store(numDissimilarChunks);
    std::int64_t num = (numDissimilarChunks[0] + numDissimilarChunks[1]);
    std::cout << "Num dissimilar chunks: " << num << "\n";
    std::cout << "Time: " << sw.elapsed_us() << " microsec\n";

    return 0;
}*/