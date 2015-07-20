// default values
#ifndef __MAIN_H__
#define __MAIN_H__

#define DEFAULT_SCREEN_WIDTH 1024
#define DEFAULT_SCREEN_HEIGHT 768
#define HIGH_PRECISION

#ifdef HIGH_PRECISION
typedef double ScalarType;
#define TW_TYPE_SCALAR_TYPE TW_TYPE_DOUBLE
#else
typedef float ScalarType;
#define TW_TYPE_SCALAR_TYPE TW_TYPE_FLOAT
#endif

#ifdef HIGH_PRECISION
#define EPSILON 1e-15
#else
#define EPSILON 1e-6
#endif

#define LARGER_EPSILON 1e-6

// eigen
#include "Core"
#include "Dense"
#include "Sparse"

// glm
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include<iostream>
using namespace std;

struct VertexPoint
{
	glm::vec3 v;
	glm::vec3 nor;
	glm::vec2 tex;
};
#endif