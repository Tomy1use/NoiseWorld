#pragma once

#include <Math/Vector.h>
#include <Math/VectorFunc.h>
#include <Math/Matrix.h>
#include <Math/MatrixFunc.h>

struct FollowCamera
{
    Matrix matrix;
    float distXZ;
    FollowCamera(const Vector& origin, const Vector& target)
        : matrix(matrixLookAt(origin, target))
        , distXZ(0)
    {
        distXZ = length(VectorXZ(target - origin));
    }
    void update(const Vector& target)
    {
        Vector originXZ = unit(VectorXZ(matrix.origin - target)) * distXZ + target;
        matrix = matrixLookAt(VectorSetY(originXZ, matrix.origin.y), target);
    }
};
