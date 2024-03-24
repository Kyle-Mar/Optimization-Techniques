// jh fall 2019

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class Image {
    Image(const Image&) = delete;
    void operator=(const Image&) = delete;

public:

    enum class Format{
        I8=1, IA8=2, RGB8=3, RGBA8=4, RGB16=6, RGBA16=8
    };

    /**Load PNG or JPEG file*/
    Image(std::string filename);

    /** Initialize from PNG or JPEG data*/
    Image(const void* data, size_t size);

    /** Initialize from PNG or JPEG data*/
    template<typename T>
    Image(const std::vector<T>& data) : Image(data.data(), data.size()*sizeof(T) ) {}

    /** Create an empty image of the given size and format*/
    Image(unsigned w, unsigned h, Format fmt);


    unsigned width() const ;
    unsigned height() const;
    unsigned bytesPerPixel() const ;
    unsigned totalBytes() const { return width()*height()*bytesPerPixel(); }

    void flipVertically();
    void sample(float s, float t, uint8_t& r, uint8_t& g, uint8_t b, uint8_t a);
    void getPixel(unsigned x, unsigned y, std::uint8_t& r,
            std::uint8_t& g, std::uint8_t& b, std::uint8_t& a) const;
    void setPixel(unsigned x, unsigned y, std::uint8_t r,
            std::uint8_t g, std::uint8_t b, std::uint8_t a=255);
    Format format() const ;
    std::uint8_t* pixels();
    const std::uint8_t* pixels() const;
    void writePng(std::string fname) const ;
    void writeBmp(std::string fname) const;

private:
    unsigned w,h;
    unsigned Bpp;   //bytes per pixel
    Format fmt;
    std::vector<uint8_t> pixbuff;
    unsigned pixbuffOffset;
    void computePixbuffOffset();
    void initialize(const void* data, size_t size);
};
