#pragma once
#include <math2801.h>
#include <array>


namespace Shapes {
    using namespace math2801;
    struct Object {
        vec3 color;
        Object(vec3 color_) : color(color_) {}
    };

    struct Sphere : Object {
        vec3 center;
        float radius;
        Sphere(math2801::vec3 center_, float r, math2801::vec3 color_) : Object(color_), center(center_), radius(r) {}
    };

    struct Triangle : Object {
        std::array<vec3, 3> points;
        vec3 N;
        float D;
        Triangle(vec3 a, vec3 b, vec3 c, vec3 color_) : points{ a,b,c }, Object(color_)
        {
            vec3 v1 = b - a;
            vec3 v2 = c - a;
            N = normalize(cross(v1, v2));

            D = -dot(N, a);
        }
    };

    inline math2801::vec3 getEdge(const Triangle& tri, float i) {
        return tri.points[(int)(i + 1) % 3] - tri.points[i];
    }

    inline float SubArea(const Triangle& tri, const math2801::vec3& ip, float i) {
        return 0.5 * length(cross(getEdge(tri, i), (ip - tri.points[i])));
    }

    inline float Area(const Triangle& tri) {
        auto edge0 = getEdge(tri, 0);
        auto edge1 = getEdge(tri, 1);
        return 0.5 * length(cross(edge0, edge1));
    }
}

namespace GPUShapes
{
    using namespace math2801;
    struct Triangle {
        alignas(16) vec4 color;
        alignas(16) vec4 a;
        alignas(16) vec4 b;
        alignas(16) vec4 c;
    };
}