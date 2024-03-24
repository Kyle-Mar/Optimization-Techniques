#include "Shapes.h"
#include <math2801.h>
#include "ReadPLY.h"
#include "Camera.h"
#include <sstream>
#include <fstream>
#include "ReadScene.h"


void readScene(std::string filename, World* world) {
    std::ifstream in(filename, std::ios::binary);

    if (!in.good()) {
        throw std::runtime_error("NO SUCH FILE!");
    }
    std::string line;

    while (true) {
        getline(in, line);
        if (in.fail()) {
            break;
        }
        if (line.find("sphere") == 0) {
            std::istringstream iss(line);
            std::string junk;
            iss >> junk;
            auto sphere = Shapes::Sphere(math2801::vec3(0, 0, 0), 0, math2801::vec3(0, 0, 0));
            iss >> sphere.center.x;
            iss >> sphere.center.y;
            iss >> sphere.center.z;
            iss >> sphere.radius;
            iss >> sphere.color.x;
            iss >> sphere.color.y;
            iss >> sphere.color.z;

            world->spheres.push_back(sphere);
        }
        if (line.find("mesh") == 0) {
            std::istringstream iss(line);
            std::string junk;
            std::string file;
            iss >> junk;
            iss >> file;
            auto pos = math2801::vec3(0, 0, 0);
            iss >> pos.x;
            iss >> pos.y;
            iss >> pos.z;
            std::vector<Shapes::Triangle> tris = readPly(file, pos);
            world->triangles.insert(std::end(world->triangles), std::begin(tris), std::end(tris));
        }
    }
}

void readScene(std::string filename, GPUWorld* world) {
    World w;
    readScene(filename, &w);
    using namespace math2801;

    // copy data into gpu world lol
    for (int i = 0; i < w.triangles.size(); i++) {
        vec3 color = w.triangles[i].color;
        std::array<vec3, 3> tri = w.triangles[i].points;
        GPUShapes::Triangle newTri{
            vec4(color.x, color.y, color.z, 1),
            vec4(tri[0][0], tri[0][1], tri[0][2], 0),
            vec4(tri[1][0], tri[1][1], tri[1][2], 0),
            vec4(tri[2][0], tri[2][1], tri[2][2], 0)
        };
        world->triangles.push_back(newTri);
    }
}
