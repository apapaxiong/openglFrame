#ifndef _MATH_HEADERS_H_
#define _MATH_HEADERS_H_


// global header
#include "main.h"

// eigen vectors and matrices
typedef int IndexType;
typedef Eigen::Matrix<ScalarType, 3, 3, 0, 3 ,3> EigenMatrix3;
typedef Eigen::Matrix<ScalarType, 3, 1, 0, 3 ,1> EigenVector3;
typedef Eigen::Matrix<ScalarType, Eigen::Dynamic, 1> VectorX;
typedef Eigen::Matrix<ScalarType, Eigen::Dynamic, Eigen::Dynamic> Matrix;
typedef Eigen::SparseMatrix<ScalarType> SparseMatrix;
typedef Eigen::Triplet<ScalarType,IndexType> SparseMatrixTriplet;

typedef glm::vec3 glm_vector3;
typedef glm::vec2 glm_vector2;
// eigen vector accessor
#define block_vector(a) block<3,1>(3*(a), 0)

// eigen 2 glm, glm 2 eigen
glm::vec3 Eigen2GLM(const EigenVector3& eigen_vector);
EigenVector3 GLM2Eigen(const glm::vec3& glm_vector);

#endif