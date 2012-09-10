#pragma once

Matrix matrixFromOriginAxisZ(const Vector& o, const Vector& z);
Matrix matrixFromOriginAxisZ(const Vector& o, const Vector& z, const Vector &y);
Matrix matrixFromOriginAxisY(const Vector& o, const Vector& y);
Matrix matrixLookAt(const Vector& from, const Vector& at);
