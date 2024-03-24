#pragma once
#include <SDL.h>
#include <iostream>
#include <stdint.h>
#include <framework/math2801.h>


struct Camera {
	math2801::vec3 up, right, look, eye;
	float fovRadians; // fov
};

