#include "Wave.h"
#include <iostream>
#include "Stopwatch.h"

//int main(int argc, char* argv[])
//{
//    if( argc != 4 ){
//        std::cout << "Syntax: " << argv[0] << " filename decayFactor delayTime\n";
//        return 1;
//    }
//    
//    std::string filename = argv[1];
//    float decayFactor = atof(argv[2]);
//    float delayTime = atof(argv[3]);
//    
//    Wave w(filename);
//    if( w.format.format != Wave::FormatCode::FLOAT ){
//        std::cout << "Not a float wave\n";
//        return 1;
//    }
//    
//    int delayTimeFrames = int(delayTime * w.format.samplesPerSecond);
//    int delayTimeFloats = delayTimeFrames * w.format.numChannels;
//    unsigned totalFloats = w.numFrames * w.format.bytesPerFrame / 4;
//
//    //you can ignore the fact that the output file should
//    //be lengthened to account for the echo at the end.
//
//    Stopwatch swatch;
//    swatch.start();
//    float* input = (float*) w.data() ;
//    float* output = (float*) w.data();
//    output += delayTimeFloats;
//    totalFloats -= delayTimeFloats;     //don't go past end
//    for(unsigned i=delayTimeFloats;i<totalFloats;i++){
//        *output += decayFactor * *input;
//        input++;
//        output++;
//    }
//    swatch.stop();
//    
//    std::cout << "Total time: " << swatch.elapsed_ms() << " msec\n";
//    w.write("out.wav");
//    
//    return 0;
//}
