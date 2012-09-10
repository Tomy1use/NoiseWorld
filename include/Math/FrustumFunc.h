#pragma once

#include <Math/Vector.h>
#include <Math/Matrix.h>
#include <Math/Frustum.h>

bool isSpherePartInFrustum(const Vector& center, float radius, const Frustum& frustum);
Frustum transform(const Frustum& in, const Matrix& matrix);
Frustum pyramidFrustum(float width, float height, float focalDistance);
Frustum corridorFrustum(float width, float height);
