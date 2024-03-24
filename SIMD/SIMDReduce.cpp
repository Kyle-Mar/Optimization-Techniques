#include "Wave.h"
#include "xmm.h"
#include "Stopwatch.h"
#include <iostream>

using namespace std;

/*int main(int argc, char* argv[])
{
    string filename = argv[1];
    float factor = atof(argv[2]);
    xmm factor_xmm = xmm(factor);

    Wave w(filename);
    if (w.format.format != Wave::FormatCode::FLOAT) {
        cout << "Not a float wave\n";
        return 1;
    }
    Stopwatch swatch;
    swatch.start();
    
    unsigned totalFloats = w.numFrames * w.format.numChannels / 16;
    
    xmm* f = (xmm*)w.data();
    for (unsigned i = 0; i < totalFloats; i++) {
        //std::cout << &f[i] << std::endl;
        //f[i] = f[i] * factor_xmm;
        //f[i + 1] = f[i + 1] * factor_xmm;
    }
    /*float* f = (float*)w.data();
    unsigned totalFloats = w.numFrames * w.format.numChannels;
    for (unsigned i = 0; i < totalFloats; i++) {
        f[i] *= factor;
    }
    swatch.stop();
    cout << swatch.elapsed_us() << "usec\n";
    w.write("out.wav");
    return 0;
}*/