// jh fall 2019 

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

class  GreyscaleImage;

class Image {
    Image(const Image&) = delete;
    void operator=(const Image&) = delete;
    
public:
    /**Load PNG or JPEG file*/
    Image(std::string filename, bool align32=true);
 
    /** Initialize from PNG or JPEG data*/
    Image(const void* data, size_t size, bool align32);

    /** Initialize from PNG or JPEG data*/
    template<typename T>
    Image(const std::vector<T>& data, bool align32=true) : Image(data.data(), data.size()*sizeof(T) ) {}
     
    /** Create an empty image of the given size and format*/
    Image(unsigned w, unsigned h, std::string fmt, bool align32=true);
     
    
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
    std::string format() const ;
    std::uint8_t* pixels();
    const std::uint8_t* pixels() const;
    void writePng(std::string fname) const ;
    void writeBmp(std::string fname) const;
    
private:
    bool align32;
    unsigned w,h;
    unsigned Bpp;   //bytes per pixel
    std::string fmt;
    std::vector<uint8_t> pixbuff;
    unsigned pixbuffOffset;
    
    void computePixbuffOffset();
    void initialize(const void* data, size_t size);
    friend class GreyscaleImage;
};

class GreyscaleImage{
    GreyscaleImage(const GreyscaleImage&) = delete;
    void operator=(const GreyscaleImage&) = delete;

public:
    /**Load PNG or JPEG file*/
    GreyscaleImage(std::string filename);
 
    /** Initialize from PNG or JPEG data*/
    GreyscaleImage(const void* data, size_t size);

    /** Create an empty image of the given size*/
    GreyscaleImage(unsigned w, unsigned h);
     
    
    unsigned width() const ;
    unsigned height() const;
    unsigned bytesPerPixel() const ;
    unsigned totalBytes() const { return width()*height()*bytesPerPixel(); }
    std::uint8_t getPixel(unsigned x, unsigned y) const;
    void setPixel(unsigned x, unsigned y, std::uint8_t v);
    std::uint8_t* pixels();
    const std::uint8_t* pixels() const;
    void writePng(std::string fname) const ;
    void writeBmp(std::string fname) const;
    
private:
    void createFrom(Image& src);
    std::shared_ptr<Image> img;
}; 
