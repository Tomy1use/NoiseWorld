#pragma once

struct PerlinNoise
{
	static const int PermTableSize = 256;
	int perm[PermTableSize];
	PerlinNoise();
	float operator() (float x, float y) const;
	float operator() (float x, float y, float z) const;
};
