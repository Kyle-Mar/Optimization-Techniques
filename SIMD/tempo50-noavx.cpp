#include "Wave.h"
#include <iostream>
#include "Stopwatch.h"

using namespace std;

//int main(int argc, char* argv[])
//{
//    string filename = argv[1];
//    
//    Wave inwave(filename);
//    if( inwave.format.format != 1 || inwave.format.numChannels != 1 || inwave.format.bitsPerSample != 8 ){
//        cout << "Not an 8 bit mono wave\n";
//        return 1;
//    }
//    
//    int nf = inwave.numFrames/2;
//    Wave outwave(inwave.format, inwave.numFrames/2  );
//    
//    std::uint8_t* inputData = reinterpret_cast<std::uint8_t*>(inwave.data());
//    std::uint8_t* outputData = reinterpret_cast<std::uint8_t*>(outwave.data());
//    
//    Stopwatch swatch;
//    swatch.start();
//    for(unsigned i=0,j=0;j<nf;i+=2,j++){
//        outputData[j] = inputData[i];
//    }
//    swatch.stop();
//    std::cout << swatch.elapsed_us() << " elapsed us" << std::endl;
//    outwave.write("out.wav");
//    
//    return 0;
//}
