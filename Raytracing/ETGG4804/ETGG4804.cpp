#include <SDL.h>
#include <iostream>
#include <stdint.h>
#include <math2801.h>
#include "Camera.h"
#include "Pixel.h"
#include "Utils.h"
#include "Consts.h"
#include "Shapes.h"
#include "ReadPLY.h"
#include "ReadScene.h"

using namespace math2801;

Pixel computePixelColor(World world, int xi, int yi);
vec3 getRayColor(const vec3& rayStart, const vec3& rayDirection, const std::vector<Shapes::Sphere>& spheres, const std::vector<Shapes::Triangle>& triangles, const vec3& lightPosition);
vec3 shadePixel(const vec3& lightPosition, const vec3& rayStart, const vec3& ip, const vec3& normal, const vec3& objcolor);
void raySphereIntersection(const vec3& rayStart, const vec3& rayDir, Shapes::Sphere sph, float& dist, vec3& ip);
void rayTriangleIntersection(const vec3& rayStart, const vec3& rayDir, Shapes::Triangle tri, float& dist, vec3& ip);
vec3 getTriangleNormal(const Shapes::Triangle& tri);

vec3 shadePixel(const vec3& lightPosition,
    const vec3& rayStart, const vec3& ip,
    const vec3& normal, const vec3& objColor)
{
    vec3 L = normalize(lightPosition - ip);
    float diffuse = dot(L, normal);
    //std::cout << normal << std::endl;
    diffuse = std::max(diffuse, 0.0f);
    float specular;
    if (diffuse > 0.0f) {
        vec3 V = normalize(rayStart - ip);
        vec3 R = reflect(-L, normal);
        specular = dot(V, R);
        specular = std::max(specular, 0.0f);
        specular = powf(specular, 16.0f);
    }
    else {
        specular = 0.0;
    }
    vec3 result = objColor * diffuse + specular * vec3(1, 1, 1);
    return min(result, vec3(1, 1, 1));
}

vec3 getRayColor(const vec3& rayStart,
    const vec3& rayDirection,
    const std::vector<Shapes::Sphere>& spheres,
    const std::vector<Shapes::Triangle>& triangles,
    const vec3& lightPosition) {

    float closestDistance = std::numeric_limits<float>::infinity();
    const Shapes::Object* closestObject = nullptr;
    vec3 closestIP;
    vec3 closestNormal;

    for (const Shapes::Sphere& sph : spheres) {
        float dist;
        vec3 ip;
        raySphereIntersection(rayStart, rayDirection, sph, dist, ip);
        if (dist < closestDistance) {
            closestDistance = dist;
            closestObject = &sph;
            //std::cout << ip - sph.center << std::endl;
            closestNormal = normalize(ip - sph.center);
            closestIP = ip;
        }
    }

    for (const Shapes::Triangle& tri : triangles) {
        float dist;
        vec3 ip;
        rayTriangleIntersection(rayStart, rayDirection, tri, dist, ip);
        if (dist < closestDistance) {
            closestDistance = dist;
            closestObject = &tri;
            closestNormal = getTriangleNormal(tri);
            closestIP = ip;
        }
    }

    if (!closestObject) {
        return vec3(0.2, 0.4, 0.6);
    }
    else {

        vec3 temp = shadePixel(lightPosition, rayStart, closestIP, closestNormal, closestObject->color);
        //std::cout << temp.x << "," << temp.y << "," << temp.z << std::endl;
        return temp;
    }
}

Pixel computePixelColor(World world, int xi, int yi) {
    vec3 v = world.camera.look;
    v += Utils::lerp(
            -tan(world.camera.fovRadians),
            tan(world.camera.fovRadians),
            xi / (WIDTH - 1.0f)
        ) * world.camera.right;
    v += Utils::lerp(
        -tan(world.camera.fovRadians),
        tan(world.camera.fovRadians),
        yi / (HEIGHT - 1.0f)
    ) * world.camera.up;

    vec3 color = getRayColor(
        world.camera.eye, //raystart;
        v,                //ray direction
        world.spheres,    //objects to trace
        world.triangles,
        world.lightPosition // light location
    );

    color = min(color, vec3(1, 1, 1));
    color = max(color, vec3(0, 0, 0));
    color = 255 * color;
    return Pixel(
        (std::uint8_t)color.x,
        (std::uint8_t)color.y,
        (std::uint8_t)color.z,
        255
    );

}

vec3 getTriangleNormal(const Shapes::Triangle& tri)
{
    //assume vertices go in CCW order
    //       0
    //      /|
    //     / |
    //    /  |
    //  1/___|2

    vec3 v1 = tri.points[1] - tri.points[0];
    vec3 v2 = tri.points[2] - tri.points[0];
    vec3 n = normalize(cross(v1, v2));
    return n;
}

void rayTriangleIntersection(
    const vec3& rayStart,
    const vec3& rayDir,
    Shapes::Triangle tri,
    float& dist, vec3& ip) {

#pragma region PlaneIntersection
    vec3 normal = getTriangleNormal(tri);
    float denom = dot(normal, rayDir);

    // in this case, the ray is either parallel or not intersecting.
    if (denom <= 0) {
        dist = INFINITY;
    }

    //solve for the point on the plane on which the triangle exists.
    float d = -dot(normal, tri.points[0]);
    float t =  ( - (d + dot(normal, rayStart)))/ denom;

    dist = t;
    ip = rayStart + t * rayDir;
#pragma endregion

#pragma region PointInTriangle

    float area = Shapes::Area(tri);
    float total = Shapes::SubArea(tri, ip, 0) + Shapes::SubArea(tri, ip, 1) + Shapes::SubArea(tri, ip, 2);

    // is point outside of triangle
    // is it close for floating point error.
    if ((total / area) > 1.001) {
        dist = INFINITY;
    }else{
        //std::cout << "INTERSECT" << std::endl;
    }

#pragma endregion 
}

void raySphereIntersection(
    const vec3& rayStart,
    const vec3& rayDir,
    Shapes::Sphere sph,
    float& dist, vec3& ip)
{
    //using quadratic formula to solve for point on sphere.
    float a = dot(rayDir, rayDir);
    const vec3& q = rayStart - sph.center; // vector from center to raystart.
    float b = 2 * dot(q, rayDir);
    float c = dot(q, q) - (sph.radius * sph.radius);

    float discriminant = b * b - 4 * a * c;

    if(discriminant < 0){
        dist = std::numeric_limits<float>::infinity();
        return;
    }

    float inner = sqrt(discriminant);
    float t1 = (-b - inner) / (2*a);
    float t2 = (-b + inner) / (2*a);

    if (t2 <= 0) {
        dist = INFINITY;
    }
    else if (t1 > 0) {
        dist = t1;
    }
    else {
        dist = t2;
    }
    ip = rayStart + dist * rayDir;
}

void updateSurface(SDL_Surface* surf, SDL_Surface* winsurf, SDL_Window* win) {
    SDL_BlitSurface(surf, nullptr, winsurf, nullptr);
    SDL_LockSurface(surf);
    SDL_UpdateWindowSurface(win);
    SDL_UnlockSurface(surf);
}

void pollEvents() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
            SDL_Quit();
            std::exit(0);
        }
    }
}

void computeImage(SDL_Surface* surf, SDL_Surface* winsurf, SDL_Window* win, World world) {
    auto startTime = SDL_GetTicks();
    auto nextUpdateTime = startTime + 10;

    for (int yi = HEIGHT-1; yi > 0; --yi) {
        for (int xi = 0; xi < WIDTH; ++xi) {
            Pixel p = computePixelColor(world, xi, yi);
            SDL_LockSurface(surf);
            set_pixel(surf, xi, yi, p);
            SDL_UnlockSurface(surf);
            if (SDL_GetTicks() >= nextUpdateTime) {
                updateSurface(surf, winsurf, win);
                pollEvents();
                nextUpdateTime += 10;
            }
        }
    }

    updateSurface(surf, winsurf, win);
    
    auto endTime = SDL_GetTicks();
    std::cout << "Total Time: " << endTime - startTime << "milliseconds" << std::endl;
    
}

/*int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* win = SDL_CreateWindow(
        "ETGG4804",
        50, 50,
        WIDTH, HEIGHT,
        0);

    if (!win) {
        std::cerr << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(
        0,
        WIDTH, HEIGHT,
        32,
        SDL_PIXELFORMAT_RGBA32
    );

    SDL_Surface* winsurf = SDL_GetWindowSurface(win);

    World world;
    world.camera.eye = vec3(0, 0, 0);
    world.camera.right = vec3(1, 0, 0);
    world.camera.up = vec3(0, 1, 0);
    world.camera.look = vec3(0, 0, -1);
    world.camera.fovRadians = 35.0 / 180.0 * PI;
    world.backgroundColor = vec3(0.2, 0.4, 0.6);

    world.lightPosition = vec3(5, 5, 5);

    readScene("scene.txt", &world);



   // world.triangles = readPly("torus.ply", vec3(0,0,0));

    /*world.triangles.push_back(Shapes::Triangle(vec3(0, 5, -10),
        vec3(-2, -2, -10),
        vec3(2, -1, -10),
        vec3(1, 1, 0)
    ));
   /* world.spheres.push_back(Shapes::Sphere(vec3(-2, 0, -4), 0.5, vec3(0, 1, 0)));
    world.spheres.push_back(Shapes::Sphere(vec3(0, 0, -4), 1, vec3(1, 0, 0)));
    world.spheres.push_back(Shapes::Sphere(vec3(2, 0, -4), 0.5, vec3(0, 0, 1)));



    computeImage(surf, winsurf, win, world);

    SDL_Event ev;
    while (1) {
        SDL_WaitEvent(&ev);
        if (ev.type == SDL_QUIT) {
            SDL_Quit();
            return 0;
        }
    }

    return 0;
}*/
