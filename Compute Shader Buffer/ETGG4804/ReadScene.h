#pragma once
#include "Camera.h"
#include <framework/math2801.h>
#include "Shapes.h"
#include <vector>

struct World {
    Camera camera;
    math2801::vec3 backgroundColor;
    math2801::vec3 lightPosition;
    std::vector<Shapes::Sphere> spheres;
    std::vector<Shapes::Triangle> triangles;
};

void readScene(std::string filename, World* world);