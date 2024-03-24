#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <cstdint>
#include "Shapes.h"
#include <math2801.h>
#pragma pack(push,1)
struct Vertex {
    float x, y, z;
    float nx, ny, nz;
    float s, t;
    std::uint8_t r, g, b, a;
};
struct Face {
    std::uint8_t num;
    std::uint32_t v0, v1, v2;
};
#pragma pack(pop)

std::vector<Shapes::Triangle> readPly(std::string filename, math2801::vec3 location)
{
    std::ifstream in(filename, std::ios::binary);

    if (!in.good())
        throw std::runtime_error("NO SUCH FILE!");

    std::string line;
    getline(in, line);
    if (line != "ply")
        throw std::runtime_error("THIS IS NOT A PLY FILE");

    int numVerts;
    int numFaces;
    while (true) {
        getline(in, line);
        if (in.fail())
            throw std::runtime_error("EARLY EOF");

        if (line.find("element vertex") == 0) {
            std::istringstream iss(line);
            std::string junk;
            iss >> junk;        //element
            iss >> junk;        //vertex
            iss >> numVerts;
        }
        else if (line.find("element face") == 0) {
            std::istringstream iss(line);
            std::string junk;
            iss >> junk;
            iss >> junk;
            iss >> numFaces;
        }
        else if (line.find("end_header") == 0) {
            break;
        }
        else {
            //ignore line
        }
    }

    std::vector<Vertex> verts(numVerts);
    in.read(reinterpret_cast<char*>(verts.data()),
        numVerts * sizeof(Vertex));

    std::vector<Shapes::Triangle> triangles;

    for (int i = 0; i < numFaces; ++i) {
        Face f;
        in.read((char*)&f, sizeof(f));
        if (f.num != 3)
            throw std::runtime_error("NOT A TRIANGLE");
        math2801::vec3 a, b, c;
        Vertex tmp = verts[f.v0];
        a = math2801::vec3(tmp.x + location.x, tmp.y + location.y, tmp.z + location.z);
        Vertex tmp1 = verts[f.v1];
        b = math2801::vec3(tmp1.x + location.x, tmp1.y + location.y, tmp1.z + location.z);
        Vertex tmp2 = verts[f.v2];
        c = math2801::vec3(tmp2.x + location.x, tmp2.y + location.y, tmp2.z + location.z);

        triangles.push_back(Shapes::Triangle(
            a, b, c,
            math2801::vec3(tmp.r / 255.0, tmp.g / 255.0, tmp.b / 255.0)
        ));
    }
    return triangles;
}