// jh 2018
//02-jul-2018: Tested with Windows: OK
//05-oct-2018: Tested with Linux: OK

#include "Image.h"

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



void encodePNG(int w, int h, Image::Format fmt, void* pixels, std::vector<std::uint8_t> buffer);
void encodeBMP(int w, int h, Image::Format fmt, void* pixels, std::vector<std::uint8_t> buffer);


/**Load PNG or JPEG file*/
Image::Image(std::string filename)
{
    std::ifstream in(filename,std::ios::binary);
    if( !in.good() )
        throw std::runtime_error("Could not open image file "+filename);
    in.seekg(0,std::ios::end);
    auto sz = in.tellg();
    in.seekg(0);
    std::vector<char> D(sz);
    in.read( D.data(), D.size() );
    if( in.fail() )
        throw std::runtime_error("Could not read image file "+filename);
    initialize(D.data(), (unsigned)D.size());
}

/** Initialize from PNG or JPEG data*/
Image::Image(const void* data, size_t size){
    initialize(data,size);
}


/** Create an empty image of the given size and format*/
Image::Image(unsigned w, unsigned h, Format fmt_)
{
    this->w=w;
    this->h=h;
    this->fmt=fmt_;

    switch(fmt_){
        case Format::RGB8:
            Bpp=3; break;
        case Format::RGBA8:
            Bpp=4; break;
        case Format::RGB16:
            Bpp=6; break;
        case Format::RGBA16:
            Bpp=8; break;
        case Format::I8:
            Bpp=1; break;
        case Format::IA8:
            Bpp=2; break;
        default:
            throw new std::runtime_error("Unknown format");
    }
    pixbuff.resize(w*h*Bpp+32);
    std::memset(pixbuff.data(),0,pixbuff.size()*sizeof(pixbuff[0]));
    computePixbuffOffset();
}

unsigned Image::width() const {
    return w;
}

unsigned Image::height() const {
    return h;
}

unsigned Image::bytesPerPixel() const {
    return Bpp;
}

void Image::flipVertically(){
    using namespace std;
    unsigned bytesPerRow = width()*bytesPerPixel();
    vector<char> tmp(bytesPerRow);
    char* sp = (char*) pixels();
    char* dp = (char*) pixels();
    dp += bytesPerRow * (height()-1);
    unsigned H=height()/2;
    for(unsigned y=0;y<H;++y){
        memcpy(tmp.data(),sp,bytesPerRow);
        memcpy(sp,dp,bytesPerRow);
        memcpy(dp,tmp.data(),bytesPerRow);
        sp += bytesPerRow;
        dp -= bytesPerRow;
    }
}

namespace {
    #pragma pack(push,1)
    struct I8Pixel{
        std::uint8_t i;
    };
    struct IA8Pixel{
        std::uint8_t i;
        std::uint8_t a;
    };
    struct RGB8Pixel{
        std::uint8_t r,g,b;
    };
    struct RGBA8Pixel{
        std::uint8_t r,g,b,a;
    };
    struct RGB16Pixel{
        std::uint16_t r,g,b;
    };
    struct RGBA16Pixel{
        std::uint16_t r,g,b,a;
    };
    #pragma pack(pop)
};

void Image::sample(float s, float t, uint8_t& r, uint8_t& g, uint8_t b, uint8_t a){
    //float s1=s, t1=t;
    if( s < 0 )
        s = 1.0f + float(s - float(int(s)));
    if( t < 0 )
        t = 1.0f + float(t - float(int(t)));

    s = std::fmod(s,1.0f);
    t = std::fmod(t,1.0f);
    int x = int( 0.5f + s * float(w));
    int y = int( 0.5f + t * float(h));
    if( x < 0 ) x = 0;
    else if( unsigned(x) >= w ) x=w-1;
    if( y < 0 ) y = 0;
    else if( unsigned(y) >= h ) y=h-1;
    getPixel( unsigned(x),unsigned(y),r,g,b,a);
}

void Image::getPixel(unsigned x, unsigned y, std::uint8_t& r,
        std::uint8_t& g, std::uint8_t& b, std::uint8_t& a) const{

    unsigned idx = y*w+x;

    switch(Bpp){
        case 1:
        {
            std::uint8_t* p = (std::uint8_t*) pixels();
            r=g=b=p[idx];
            a=255;
            break;
        }
        case 2:
        {
            //ia8
            //~ std::assert(fmt == "RGB8");
            IA8Pixel* p = (IA8Pixel*)pixels();
            r=g=b=p[idx].i;
            a=p[idx].a;
            break;
        }
        case 3:
        {
            //rgb8
            //~ std::assert(fmt == "RGB8");
            RGB8Pixel* p = (RGB8Pixel*)pixels();
            r=p[idx].r;
            g=p[idx].g;
            b=p[idx].b;
            a=255;
            break;
        }
        case 4:
        {
            //rgba8
            //~ std::assert(fmt == "RGBA8");
            RGBA8Pixel* p = (RGBA8Pixel*)pixels();
            r=p[idx].r;
            g=p[idx].g;
            b=p[idx].b;
            a=p[idx].a;
            break;
        }
        case 6:
        {
            //rgb16
            //~ std::assert(fmt == "RGB16");
            RGB16Pixel* p = (RGB16Pixel*)pixels();
            r=uint8_t(p[idx].r>>8);
            g=uint8_t(p[idx].g>>8);
            b=uint8_t(p[idx].b>>8);
            a=255;
            break;
        }
        case 8:
        {
            //rgba16
            //~ std::assert(fmt == "RGBA16");
            RGBA16Pixel* p = (RGBA16Pixel*)pixels();
            r=uint8_t(p[idx].r>>8);
            g=uint8_t(p[idx].g>>8);
            b=uint8_t(p[idx].b>>8);
            a=uint8_t(p[idx].a>>8);
            break;
        }
        default:
            throw std::runtime_error("Bad Bpp (2):" + std::to_string(Bpp));
    }
}

void Image::setPixel(unsigned x, unsigned y, std::uint8_t r,
        std::uint8_t g, std::uint8_t b, std::uint8_t a){

    unsigned idx = y*w+x;

    switch(Bpp){
        case 1:
        {
            std::uint8_t* p = (std::uint8_t*) pixels();
            p[idx] = r;
            break;
        }
        case 2:
        {
            //ia8
            //~ std::assert(fmt == "RGB8");
            IA8Pixel* p = (IA8Pixel*)pixels();
            p[idx].i=r;
            p[idx].a=a;
            break;
        }
        case 3:
        {
            //rgb8
            //~ std::assert(fmt == "RGB8");
            RGB8Pixel* p = (RGB8Pixel*)pixels();
            p[idx].r=r;
            p[idx].g=g;
            p[idx].b=b;
            break;
        }
        case 4:
        {
            //rgba8
            //~ std::assert(fmt == "RGBA8");
            RGBA8Pixel* p = (RGBA8Pixel*)pixels();
            p[idx].r=r;
            p[idx].g=g;
            p[idx].b=b;
            p[idx].a=a;
            break;
        }
        case 6:
        {
            //rgb16
            //~ std::assert(fmt == "RGB16");
            RGB16Pixel* p = (RGB16Pixel*)pixels();
            p[idx].r=uint16_t(r<<8);
            p[idx].g=uint16_t(g<<8);
            p[idx].b=uint16_t(b<<8);
            break;
        }
        case 8:
        {
            //rgba16
            //~ std::assert(fmt == "RGBA16");
            RGBA16Pixel* p = (RGBA16Pixel*)pixels();
            p[idx].r=uint16_t(r<<8);
            p[idx].g=uint16_t(g<<8);
            p[idx].b=uint16_t(b<<8);
            p[idx].a=uint16_t(a<<8);
            break;
        }
        default:
            throw std::runtime_error("Bad Bpp (1):" + std::to_string(Bpp));
    }
}

Image::Format Image::format() const {
    return fmt;
}
std::uint8_t* Image::pixels(){
    return (uint8_t*)(pixbuff.data()+pixbuffOffset);
}
const std::uint8_t* Image::pixels() const {
    return (uint8_t*)(pixbuff.data()+pixbuffOffset);
}

//~ std::uint8_t* Image::operator()(int x, int y){
    //~ return pixels()+y*w*4+x*4;
//~ }

static void writeFunc(void* v, void* data, int size)
{
    std::vector<std::uint8_t>* pix = (std::vector<std::uint8_t>*)v;
    std::uint8_t* p = (std::uint8_t*) data;
    pix->insert(pix->end(), p, p+size );
}

void Image::writePng(std::string fname) const {
    std::vector<std::uint8_t> pix;
         //~ int stbi_write_png_to_func(stbi_write_func *func, void *context, int w, int h, int comp, const void  *data, int stride_in_bytes);
    int rv = stbi_write_png_to_func( writeFunc, &pix, this->w, this->h, this->Bpp, this->pixels(), this->w*this->Bpp );
    if( !rv ){
        throw std::runtime_error("PNG write error");
    }
    std::ofstream out(fname,std::ios::binary);
    out.write((char*)pix.data(),pix.size());
    return;
}
void Image::writeBmp(std::string fname) const {
    std::vector<std::uint8_t> pix;
    stbi_write_bmp_to_func( writeFunc, &pix, this->w, this->h, this->Bpp, this->pixels() );
    std::ofstream out(fname,std::ios::binary);
    out.write((char*)pix.data(),pix.size());
    return;
}

void Image::computePixbuffOffset(){
    uint64_t p = (uint64_t)pixbuff.data();
    uint64_t q = p + 31;
    q &= ~31;
    pixbuffOffset = (unsigned)(q-p);
    assert(pixbuffOffset < 32 );
    std::uint64_t X = (std::uint64_t)(pixels());
    assert( (X & 31) == 0 );
}

void Image::initialize(const void* data, size_t size){
    int w,h,chan;
    stbi_uc* decoded = stbi_load_from_memory( (stbi_uc*) data, (int) size,
        &w,&h,&chan,4);

    //force to rgba format
    chan=4;

    if( decoded == nullptr ){
        std::string reason = stbi_failure_reason();
        std::cerr << reason << "\n";
        throw std::runtime_error(reason);
    } else {
        switch(chan){
            case 1:
                this->fmt = Format::I8;
                this->Bpp = 1;
                break;
            case 2:
                this->fmt = Format::IA8;
                this->Bpp = 2;
                break;
            case 3:
                this->fmt = Format::RGB8;
                this->Bpp = 3;
                break;
            case 4:
                this->fmt = Format::RGBA8;
                this->Bpp = 4;
                break;
            case 6:
                this->fmt = Format::RGB16;
                this->Bpp = 6;
                break;
            case 8:
                this->fmt = Format::RGBA16;
                this->Bpp = 8;
                break;
            default:
                throw std::runtime_error("Unexpected format");
        }

        this->w=w;
        this->h=h;
        std::uint64_t sz = w*h*chan;
        this->pixbuff.resize( sz+32 );
        this->computePixbuffOffset();
        std::memcpy( this->pixels(), decoded, sz );
        stbi_image_free(decoded);
    }
}
