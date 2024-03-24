#include "Wave.h"
#include <iostream>
#include "Stopwatch.h"
#include "xmm.h"

//int main(int argc, char* argv[])
//{
//    if (argc != 4) {
//        std::cout << "Syntax: " << argv[0] << " filename decayFactor delayTime\n";
//        return 1;
//    }
//
//    std::string filename = argv[1];
//    xmm decayFactor = xmm(atof(argv[2]));
//    float delayTime = xmm(atof(argv[3]));
//
//    Wave w(filename);
//    if (w.format.format != Wave::FormatCode::FLOAT) {
//        std::cout << "Not a float wave\n";
//        return 1;
//    }
//
//    int delayTimeFrames = int(delayTime * w.format.samplesPerSecond);
//    int delayTimeFloats = delayTimeFrames * w.format.numChannels / 4;
//    unsigned totalFloats = w.numFrames * w.format.bytesPerFrame / 16;
//
//    //you can ignore the fact that the output file should
//    //be lengthened to account for the echo at the end.
//
//    Stopwatch swatch;
//    swatch.start();
//    xmm* input = (xmm*)w.data();
//    xmm* output = (xmm*)w.data();
//    output += delayTimeFloats;
//    totalFloats -= delayTimeFloats;     //don't go past end
//    for (unsigned i = delayTimeFloats; i < totalFloats; i++) {
//        output[i] = output[i] + decayFactor * input[i];
//    }
//    swatch.stop();
//
//    std::cout << "Total time: " << swatch.elapsed_ms() << " msec\n";
//    w.write("out.wav");
//
//    return 0;
//}
