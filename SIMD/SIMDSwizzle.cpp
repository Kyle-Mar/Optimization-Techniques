#include "Wave.h"
#include "xmm.h"
#include "Stopwatch.h"
#include <iostream>

using namespace std;

//int main(int argc, char* argv[])
//{
//    string filename = argv[1];
//    xmmi8 mask = xmmi8(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
//    Wave w(filename);
//    Stopwatch swatch;
//    swatch.start();
//
//    unsigned totalFloats = w.numFrames * w.format.numChannels / 16;
//
//    int8_t* f = (int8_t*)w.data();
//    for (unsigned i = 0; i < totalFloats; f+=16, i+=1) {
//        xmmi8 tmp = xmmi8(f);
//;       xmmi8 x = tmp.shuffle(mask);
//        x.store(f);
//    }
//    swatch.stop();
//    cout << swatch.elapsed_us() << "usec\n";
//    w.write("out.wav");
//    return 0;
//}