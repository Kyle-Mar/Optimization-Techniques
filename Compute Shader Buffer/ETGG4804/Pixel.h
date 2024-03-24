#pragma once
#include <SDL.h>
#include <iostream>
#include <stdint.h>
#include <framework/math2801.h>
#include "Consts.h"

#pragma pack(push, 1)

struct Pixel {
    std::uint8_t r, g, b, a;

    Pixel(){

    }

    Pixel(auto r_, auto g_, auto b_, auto a_) :
        r(r_), g(g_), b(b_), a(a_) {}
    Pixel(auto c) :
        r(c), g(c), b(c), a(c) {}
};
#pragma pack(pop)


void set_pixel(SDL_Surface* surf, int x, int y, Pixel color);

void set_pixel(SDL_Surface* surf, int x, int y, Pixel color) {
    y = HEIGHT - y - 1;
    uint8_t* p = (uint8_t*)surf->pixels;
    p += y * surf->pitch; // pitch is row size;
    p += x * 4;
    *p++ = color.r;
    *p++ = color.g;
    *p++ = color.b;
    *p++ = color.a;
}
