#pragma once
#include <string>
#include <vector>
#include <framework/math2801.h>
#include "Shapes.h"

std::vector<Shapes::Triangle> readPly(std::string filename, math2801::vec3 location);