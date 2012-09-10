#pragma once

#include <Math/NewellNormal.h>

const float BuildingStoryHeight = 3.f;

struct BuildingRenderPiece;

struct Building
{
	Building* next;
	BuildingRenderPiece* renderPiece;
	static const int MaxBasePoints = 64;
	int basePointCount;
	Vector* basePoints;
	Vector pointNormals[MaxBasePoints];
	Vector edgeNormals[MaxBasePoints];
	int storyCount;
	int profilePointCount;
	Vector* profile;//array of [wall_extrusion, height, normal_y]
	Building()
		: next(NULL), renderPiece(NULL), basePointCount(0)
		, basePoints(new Vector[MaxBasePoints])
		, storyCount(0)
		, profilePointCount(0)
		, profile(NULL)
	{
	}
	~Building()
	{
		delete[] profile;
		delete[] basePoints;
	}
	void cutAngles()
	{
		int j = 0;
		Vector* newBasePoints = new Vector[basePointCount*2];
		for(int i=0; i<basePointCount; i++){
			Vector e1 = unit(basePoints[(i+1)%basePointCount] - basePoints[i]);
			Vector e2 = unit(basePoints[(i+2)%basePointCount] - basePoints[(i+1)%basePointCount]);
			if(dot(e1, e2) < .5f){
				newBasePoints[j++] = basePoints[(i+1)%basePointCount] - e1;
				newBasePoints[j++] = basePoints[(i+1)%basePointCount] + e2;
			}else{
				newBasePoints[j++] = basePoints[(i+1)%basePointCount];
			}
		}
		delete[] basePoints;
		basePointCount = j;
		basePoints = newBasePoints;
	}
	void revertPointOrder()
	{
		printf("Building::revertPointOrder\n");
		for(int i=0; i<basePointCount/2; i++){
			int j = basePointCount - i - 1;
			Vector x = basePoints[i];
			basePoints[i] = basePoints[j];
			basePoints[j] = x;
		}
	}
	void fixPointOrder()
	{
		NewellNormal N;
		for(int i=0; i<basePointCount; i++) N.advance(basePoints[i]);
		printf("fixPointOrder(): Newell normal Y = %.2f\n", N.result().y);
		if(N.result().y > 0) revertPointOrder();
	}
	void calcNormals()
	{
		for(int i=0; i<basePointCount; i++){
			Vector edge = basePoints[(i+1)%basePointCount] - basePoints[i];
			edgeNormals[i] = unit(cross(UnitVectorY, edge));
		}
		for(int i=0; i<basePointCount; i++){
			pointNormals[(i+1)%basePointCount] = unit((edgeNormals[i] + edgeNormals[(i+1)%basePointCount]) / 2);
		}
	}
	void calcProfile()
	{
		profilePointCount = storyCount * 4 + 2;
		profile = new Vector[profilePointCount];
		int Ph = rand() % 3;
		for(int i=0; i<=storyCount; i++){
			float F = float(((i+Ph) / 5) % 3);
			profile[i*4+0] = Vector(.3f+F, float(i*3+0), 0);
			profile[i*4+1] = Vector(.3f+F, float(i*3+1), 0);
			if(i < storyCount){
				profile[i*4+2] = Vector(0+F, i*3+1.3f, 0);
				profile[i*4+3] = Vector(0+F, i*3+2.7f, 0);
			}
		}
		for(int i=0; i<profilePointCount-1; i++){
			profile[i].z = unit(cross(profile[i+1] - profile[i], UnitVectorZ)).y;
		}
		{//cornice
			profile[profilePointCount-1].x += 1.f;
			profile[profilePointCount-2].x += 1.f;
		}
	}
};
