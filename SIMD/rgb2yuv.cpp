
#include "Image.h"
#include "Stopwatch.h"
#include <iostream>

//#pragma pack(push,1)
//struct Pixel{
//    std::uint8_t r,g,b,a;
//};
//#pragma pack(pop)
//
//std::uint8_t clamp(float f)
//{
//    if(f>255.0)
//        return 255;
//    if(f<0.0)
//        return 0;
//    return std::uint8_t(f);
//}
//
//int main(int argc, char* argv[])
//{
//    if( argc != 2 ){
//        std::cout << "Bad arguments\n";
//        return 1;
//    }
//
//    Image img(argv[1]);
//
//    Stopwatch s;
//    s.start();
//    Pixel* p = reinterpret_cast<Pixel*>(img.pixels());
//
//    Image Yimg(img.width(),img.height(),Image::Format::I8);
//    Image Cbimg(img.width(),img.height(),Image::Format::I8);
//    Image Crimg(img.width(),img.height(),Image::Format::I8);
//
//    std::uint8_t* Y = Yimg.pixels();
//    std::uint8_t* Cb = Cbimg.pixels();
//    std::uint8_t* Cr = Crimg.pixels();
//
//    unsigned numPixels = img.width()*img.height();
//    for(unsigned i=0;i<numPixels;++i,++p,++Y,++Cb,++Cr){
//        *Y = clamp( 0.299*p->r + 0.587*p->g + 0.114*p->b );
//        *Cb = clamp(128 - 0.168736*p->r - 0.331264*p->g + 0.5*p->b);
//        *Cr = clamp(128 + 0.5*p->r - 0.418688*p->g - 0.081312*p->b);
//    }
//    s.stop();
//
//    std::cout << "Time: " << s.elapsed_us() << " microseconds\n";
//
//    Yimg.writePng("Y.png");
//    Cbimg.writePng("Cb.png");
//    Crimg.writePng("Cr.png");
//
//    return 0;
//}
