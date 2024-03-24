#include <SDL.h>
#include <iostream>
#include <stdint.h>
#include <framework/math2801.h>
#include "Camera.h"
#include "Pixel.h"
#include "Utils.h"
#include "Consts.h"
#include "Shapes.h"
#include "ReadPLY.h"
#include "ReadScene.h"
#include "ymm.h"


using namespace math2801;

inline
ymm determinant(ymm a, ymm b,
    ymm c, ymm d)
{
    return a * d - b * c;
}

ymm dot(ymm ax, ymm ay, ymm az, ymm bx, ymm by, ymm bz)
{
    return ax * bx + ay * by + az * bz;
}

void normalize_ymm(ymm& X, ymm& Y, ymm& Z)
{
    ymm len = (X * X + Y * Y + Z * Z).rsqrt();

    X = X * len;
    Y = Y * len;
    Z = Z * len;
}

void reflect(ymm incomingX, ymm incomingY, ymm incomingZ, ymm normalX, ymm normalY, ymm normalZ, ymm& Rx, ymm& Ry, ymm& Rz) {
    ymm Vx = incomingX * ymm(-1);
    ymm Vy = incomingY * ymm(-1);
    ymm Vz = incomingZ * ymm(-1);

    normalize_ymm(normalX, normalY, normalZ);
    
    ymm scalar = dot(Vx, Vy, Vz, normalX, normalY, normalZ);
    ymm Qx = scalar * normalX;
    ymm Qy = scalar * normalY;
    ymm Qz = scalar * normalZ;
    ymm Ax = Qx - Vx;
    ymm Ay = Qy - Vy;
    ymm Az = Qz - Vz;

    Rx = Vx + Ax + Ax;
    Ry = Vy + Ay + Ay;
    Rz = Vz + Az + Az;
}





void computePixelColor(World world, int xi, int yi);
void getRayColor(const vec3& rayStart,
                 const ymm& rayDirX, 
                 const ymm& rayDirY,
                 const ymm& rayDirZ,
                 const std::vector<Shapes::Sphere>& spheres,
                 const std::vector<Shapes::Triangle>& triangles,
                 const vec3& lightPosition,
                 const ymm& resultR,
                 const ymm& resultG,
                 const ymm& resultB
);
void shadePixel(const vec3& lightPosition,
                const vec3& rayStart,
                ymm ipX, ymm ipY, ymm ipZ,
                ymm normalX, ymm normalY, ymm normalZ,
                ymm colorR, ymm colorG, ymm colorB,
                ymm& resultR, ymm& resultG, ymm& resultB);

void raySphereIntersection(const vec3& rayStart, const vec3& rayDir, Shapes::Sphere sph, float& dist, vec3& ip);
void rayTriangleIntersection(const vec3& rayStart,
                             ymm rayDirX, ymm rayDirY, ymm rayDirZ,
                             const Shapes::Triangle& tri,
                             ymm& dist,                          //output
                             ymm& ipX, ymm& ipY, ymm& ipZ        //output
);
vec3 getTriangleNormal(const Shapes::Triangle& tri);

void shadePixel(const vec3& lightPosition,
    const vec3& rayStart,
    ymm ipX, ymm ipY, ymm ipZ,
    ymm normalX, ymm normalY, ymm normalZ,
    ymm colorR, ymm colorG, ymm colorB,
    ymm& resultR, ymm& resultG, ymm& resultB)
{
    ymm Lx = ymm(lightPosition.x) - ipX;
    ymm Ly = ymm(lightPosition.y) - ipY;
    ymm Lz = ymm(lightPosition.z) - ipZ;
    normalize_ymm(Lx, Ly, Lz);        



    // to be written

    ymm diffuse = dot(Lx, Ly, Lz, normalX, normalY, normalZ);
    diffuse = max(diffuse, ymm(0.0f));

    ymm Vx = ymm(rayStart.x) - ipX;
    ymm Vy = ymm(rayStart.y) - ipY;
    ymm Vz = ymm(rayStart.z) - ipZ;
    normalize_ymm(Vx, Vy, Vz);    //to be written
    
    ymm Rx, Ry, Rz;
    reflect(Lx * ymm(-1.0), Ly * ymm(-1.0), Lz * ymm(-1.0), normalX, normalY, normalZ, Rx, Ry, Rz);

    ymm specular = dot(Vx, Vy, Vz, Rx, Ry, Rz);
    specular = max(specular, ymm(0.0f));

    //raise to the 16th power
    specular = specular * specular;       //specular**2
    specular = specular * specular;       //specular**4
    specular = specular * specular;       //specular**8
    specular = specular * specular;       //specular**16

    ymm mask = (diffuse > ymm(0.0f));
    specular = ymm(0.0f).blend(specular, mask);

    resultR = colorR * diffuse + specular;
    resultG = colorG * diffuse + specular;
    resultB = colorB * diffuse + specular;
    resultR = min(resultR, ymm(1.0));
    resultG = min(resultG, ymm(1.0));
    resultB = min(resultB, ymm(1.0));

}

void getRayColor(const vec3& rayStart,
                const ymm& rayDirX,
                const ymm& rayDirY,
                const ymm& rayDirZ,
                const std::vector<Shapes::Sphere>& spheres,
                const std::vector<Shapes::Triangle>& triangles,
                const vec3& lightPosition,
                ymm& resultR,
                ymm& resultG,
                ymm& resultB) 
{

    ymm closest_distance(INFINITY);

    ymm closest_ipX, closest_ipY, closest_ipZ;
    ymm closest_normalX, closest_normalY, closest_normalZ;
    ymm closest_colorR, closest_colorG, closest_colorB;

    //for (const Shapes::Sphere& sph : spheres) {
    //    float dist;
    //    vec3 ip;
    //    raySphereIntersection(rayStart, rayDirection, sph, dist, ip);
    //    if (dist < closestDistance) {
    //        closestDistance = dist;
    //        closestObject = &sph;
    //        //std::cout << ip - sph.center << std::endl;
    //        closestNormal = normalize(ip - sph.center);
    //        closestIP = ip;
    //    }
    //}

    for (const Shapes::Triangle& tri : triangles) {
        ymm dist;
        ymm ipX, ipY, ipZ;
        rayTriangleIntersection(rayStart, rayDirX, rayDirY, rayDirZ, tri, dist, ipX, ipY, ipZ);
        ymm mask = (dist < closest_distance);
        closest_distance = closest_distance.blend(dist, mask);
        closest_ipX = closest_ipX.blend(ipX, mask);
        closest_ipY = closest_ipY.blend(ipY, mask);
        closest_ipZ = closest_ipZ.blend(ipZ, mask);
        closest_colorR = closest_colorR.blend(ymm(tri.color.x), mask);
        closest_colorG = closest_colorG.blend(ymm(tri.color.y), mask);
        closest_colorB = closest_colorB.blend(ymm(tri.color.z), mask);
        closest_normalX = closest_normalX.blend(ymm(tri.N.x), mask);
        closest_normalY = closest_normalY.blend(ymm(tri.N.y), mask);
        closest_normalZ = closest_normalZ.blend(ymm(tri.N.z), mask);
    }

    shadePixel(
        lightPosition,
        rayStart,
        closest_ipX, closest_ipY, closest_ipZ,
        closest_normalX, closest_normalY, closest_normalZ,
        closest_colorR, closest_colorG, closest_colorB,
        resultR, resultG, resultB
    );

    ymm mask = (closest_distance != ymm(INFINITY));

    //background color for "no intersection"
    ymm bgR(0.2f); 
    ymm bgG(0.4f);
    ymm bgB(0.6f);

    resultR = bgR.blend(resultR, mask);
    resultG = bgG.blend(resultG, mask);
    resultB = bgB.blend(resultB, mask);
}

void computePixelColor(const World& world, int xi, int yi,
    Pixel result[8]) {
    float t = tan(world.camera.fovRadians);
    vec3 v[8] = {
        Utils::lerp(-t,  t,  (xi) / (WIDTH - 1.0f)) * world.camera.right,
        Utils::lerp(-t,  t,  (xi + 1) / (WIDTH - 1.0f)) * world.camera.right,
        Utils::lerp(-t,  t,  (xi + 2) / (WIDTH - 1.0f)) * world.camera.right,
        Utils::lerp(-t,  t,  (xi + 3) / (WIDTH - 1.0f)) * world.camera.right,
        Utils::lerp(-t,  t,  (xi + 4) / (WIDTH - 1.0f)) * world.camera.right,
        Utils::lerp(-t,  t,  (xi + 5) / (WIDTH - 1.0f)) * world.camera.right,
        Utils::lerp(-t,  t,  (xi + 6) / (WIDTH - 1.0f)) * world.camera.right,
        Utils::lerp(-t,  t,  (xi + 7) / (WIDTH - 1.0f)) * world.camera.right,
    };

    ymm vx(v[0].x, v[1].x, v[2].x, v[3].x, v[4].x, v[5].x, v[6].x, v[7].x);
    ymm vy(v[0].y, v[1].y, v[2].y, v[3].y, v[4].y, v[5].y, v[6].y, v[7].y);
    ymm vz(v[0].z, v[1].z, v[2].z, v[3].z, v[4].z, v[5].z, v[6].z, v[7].z);

    vec3 tmp = Utils::lerp(-t, t, yi / (HEIGHT - 1.0f)) * world.camera.up;

    tmp += world.camera.look;
    vx = vx + ymm(tmp.x);
    vy = vy + ymm(tmp.y);
    vz = vz + ymm(tmp.z);

    ymm colorR, colorG, colorB;

    getRayColor(
        world.camera.eye, //raystart;
        vx, vy, vz,                //ray direction
        world.spheres,    //objects to trace
        world.triangles,
        world.lightPosition, // light location
        colorR, colorG, colorB
    );



    ymm one(1.0f);
    ymm zero(0.0f);

    colorR = min(colorR, one);
    colorG = min(colorG, one);
    colorB = min(colorB, one);
    colorR = max(colorR, zero);
    colorG = max(colorG, zero);
    colorB = max(colorB, zero);

    ymm q(255.0f);
    colorR = colorR * q;
    colorG = colorG * q;
    colorB = colorB * q;

    ymmi32 RI, GI, BI;

    RI = _mm256_cvtps_epi32(colorR);
    GI = _mm256_cvtps_epi32(colorG);
    BI = _mm256_cvtps_epi32(colorB);

    std::int32_t R[8], G[8], B[8];
    RI.store(R);
    GI.store(G);
    BI.store(B);

    for (int i = 0; i < 8; ++i) {
        result[i] = Pixel(R[i], G[i], B[i], 255);
    }
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

void rayTriangleIntersection(const vec3& rayStart,
    ymm rayDirX, ymm rayDirY, ymm rayDirZ,
    const Shapes::Triangle& tri,
    ymm& dist,                          //output
    ymm& ipX, ymm& ipY, ymm& ipZ        //output
) {
#pragma region PlaneIntersection
    dist = ymm(INFINITY);
    //solve for the point on the plane on which the triangle exists.
    ymm denom = ymm(tri.N.x) * rayDirX + ymm(tri.N.y) * rayDirY + ymm(tri.N.z) * rayDirZ;
    ymm numer = ymm(-(tri.D + dot(tri.N, rayStart)));
    ymm t = numer / denom;



    //Can ignore this!
    //    Why ?
    //    Anything(finite) divided by zero will be ∞
    //    Will end up with dist being ∞
    //    And that counts as “no intersection”
    /*if (denom <= 0) {
        dist = INFINITY;
    }*/

    dist = t;
    ipX = ymm(rayStart.x) + t * rayDirX;
    ipY = ymm(rayStart.y) + t * rayDirY;
    ipZ = ymm(rayStart.z) + t * rayDirZ;
#pragma endregion

#pragma region PointInTriangle
    vec3 p = tri.points[0], q = tri.points[1], r = tri.points[2];
    p = p - r;
    q = q - r;
    ymm ipX_ = ipX - ymm(r.x);
    ymm ipY_ = ipY - ymm(r.y);
    ymm ipZ_ = ipZ - ymm(r.z);


    float c1 = dot(q,q);
    float c2 = dot(q, p);
    float c3 = dot(p, p);
    ymm c4 = ipX_ * ymm(q.x) + ipY_ * ymm(q.y) + ipZ_ * ymm(q.z);
    ymm c5 = ipX_ * ymm(p.x) + ipY_ * ymm(p.y) + ipZ_ * ymm(p.z);

    ymm Db(determinant(c4, ymm(c2), c5, ymm(c3)));
    ymm Da(determinant(ymm(c1), c4, ymm(c2), c5));
    ymm DD(determinant(ymm(c1), ymm(c2), ymm(c2), ymm(c3)));
    ymm beta = Db / DD;
    ymm alpha = Da / DD;

    ymmi32 mask = ymmi32(beta >= ymm(0.0f));
    mask = mask & ymmi32(alpha >= ymm(0.0f));
    mask = mask & ymmi32(beta <= ymm(1.0f));
    mask = mask & ymmi32(alpha <= ymm(1.0f));
    mask = mask & ymmi32(beta + alpha <= ymm(1.0f));
    mask = mask & ymmi32(t > ymm(0.0f));
    dist = ymm(INFINITY).blend(t, ymm(mask));


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
        for (int xi = 0; xi < WIDTH; xi+=8) {
            Pixel p[8];
            computePixelColor(world, xi, yi, p);
            SDL_LockSurface(surf);
            for (int i = 0; i < 8; i++) {
                set_pixel(surf, xi + i, yi, p[i]);
            }

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

int main(int argc, char* argv[])
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



    //orld.triangles = readPly("torus.ply", vec3(0,0,0));

    /*world.triangles.push_back(Shapes::Triangle(vec3(0, 5, -10),
        vec3(-2, -2, -10),
        vec3(2, -1, -10),
        vec3(1, 1, 0)
    ));*/
   // world.spheres.push_back(Shapes::Sphere(vec3(-2, 0, -4), 0.5, vec3(0, 1, 0)));
   // world.spheres.push_back(Shapes::Sphere(vec3(0, 0, -4), 1, vec3(1, 0, 0)));
   // world.spheres.push_back(Shapes::Sphere(vec3(2, 0, -4), 0.5, vec3(0, 0, 1)));



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
}
