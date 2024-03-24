
#include "Image.h"
#include "Stopwatch.h"
#include "ymm.h"
#include "chunky.h"
#include <iostream>

ymmu8 clamp(ymmu8 x)
{
    ymmu8 mx(255);
    ymmu8 mn((uint8_t)0);
    return min(max(x, mn), mx);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Bad arguments\n";
        return 1;
    }

    Image img(argv[1]);

    Stopwatch s;
    s.start();
    
    Image Yimg(img.width(), img.height(), Image::Format::I8);
    Image Cbimg(img.width(), img.height(), Image::Format::I8);
    Image Crimg(img.width(), img.height(), Image::Format::I8);

    uint8_t* imgP = (uint8_t*)img.pixels();

    uint8_t* Y =  (uint8_t*)Yimg.pixels();
    uint8_t* Cb = (uint8_t*)Cbimg.pixels();
    uint8_t* Cr = (uint8_t*)Crimg.pixels();
    ymmu8 x(128);
    unsigned numPixels = img.width() * img.height();

    for (unsigned i = 0; i < numPixels * 4; Y += 32, Cb += 32, Cr += 32, imgP += 128, i += 128) {
        ymmu8 Yp(Y);
        ymmu8 Cbp(Cb);
        ymmu8 Crp(Cr);
        ymmu8 R;
        ymmu8 G;
        ymmu8 B;
        ymmu8 A;

        chunky_rgba_to_planes(imgP, R, G, B, A);
        Yp = (((R >> 2) + (R >> 3) - (R >> 4)) + ((G >> 1) + (G >> 4)) + (B >> 3));
        Yp = clamp(Yp);
        Yp.store(Y);

        Cbp = (x - ((R >> 3) + (R >> 5) + (R >> 6)) - ((G >> 1) - (G >> 3) - (G >> 4) - (G>>5)) + (B >> 1));
        Cbp = clamp(Cbp);
        Cbp.store(Cb);

        Crp = (x + (R >> 1) - ((G >> 1) - (G >> 4)) - (B >> 4));
        Crp = clamp(Crp);
        Crp.store(Cr);
    }

    std::cout << "Time: " << s.elapsed_us() << " microseconds\n";

    Yimg.writePng("Y.png");
    Cbimg.writePng("Cb.png");
    Crimg.writePng("Cr.png");

    return 0;
}
