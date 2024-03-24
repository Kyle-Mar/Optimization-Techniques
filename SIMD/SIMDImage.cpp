

#include "Image.h"
#include "Stopwatch.h"
#include <string>
#include <cstdint>
#include <iostream>
#include "xmm.h"


//int main(int argc, char* argv[]) {
//    std::string liveFile = argv[1];
//    std::string cgiFile = argv[2];
//
//    uint8_t r = 50, g = 55, b = 200;
//
//    Image liveImage(liveFile);
//    Image cgiImage(cgiFile);
//    if (liveImage.width() != cgiImage.width() || liveImage.height() != cgiImage.height()) {
//        std::cout << "Dimension mismatch\n";
//        return 1;
//    }
//
//    Image outputImage(liveImage.width(), liveImage.height(), "RGBA8");
//
//    Stopwatch s;
//
//    s.start();
//
//    uint8_t* liveP = liveImage.pixels();
//    uint8_t* cgiP = cgiImage.pixels();
//    uint8_t* outP = outputImage.pixels();
//    xmmu8 redGreenMax(255, 0, 55, 50, 255, 0, 55, 50, 255, 0, 55, 50, 255, 0, 55, 50);
//    xmmu8 blueAlphaMin(0, 200, 255, 255, 0, 200, 255, 255, 0, 200, 255, 255, 0, 200, 255, 255);
//
//
//    unsigned numPixels = liveImage.width() * liveImage.height();
//    for (unsigned i = 0; i < numPixels; i += 4, liveP += 16, cgiP += 16, outP += 16) {
//        xmmu8 livePixRow(liveP);
//        xmmu8 cgiPixRow(cgiP);
//        xmmu8 Q = livePixRow < redGreenMax;
//        xmmu8 Z = livePixRow > blueAlphaMin;
//        xmmu8 mask = Q | Z;
//        xmmu8 F = ((xmmu32)mask == xmmu32(0xffffffff));
//        xmmu8 final_value = _mm_blendv_epi8(livePixRow, cgiPixRow, F);
//        //xmmu8 final_value = blendv(livePixRow, cgiPixRow, F);
//        final_value.store(outP);
//    }
//
//    s.stop();
//    std::cout << "Time: " << s.elapsed_us() << " usec \n";
//    outputImage.writePng("out.png");
//    return 0;
//}

