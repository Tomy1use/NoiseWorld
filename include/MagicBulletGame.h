#pragma once

#include <Math/Matrix.h>

class btDynamicsWorld;
class btMotionState;

struct MagicBulletGame
{
	btDynamicsWorld* dynamicsWorld;
	btMotionState* myMotionState;
	
	MagicBulletGame();
	~MagicBulletGame();
	void update(float dt);
	Matrix getMyMotionState();
};
