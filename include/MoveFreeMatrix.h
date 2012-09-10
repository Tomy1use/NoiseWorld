
Matrix moveFreeMatrix(const Matrix& matrix, float dt, float Speed, float BoostFactor)
{
	Matrix motion(UnitMatrix);
	float speed = Speed * (Keys::down.leftShift ? BoostFactor : 1);
	motion.origin.x = dt * speed * ((Keys::down.d ? +1.f : 0) - (Keys::down.a ? +1.f : 0));
	motion.origin.z = dt * speed * ((Keys::down.s ? +1.f : 0) - (Keys::down.w ? +1.f : 0));
	motion.origin.y = dt * speed * ((Keys::down.q ? +1.f : 0) - (Keys::down.e ? +1.f : 0));
	float rotX = ((Keys::down.up ? +1.f : 0) - (Keys::down.down ? +1.f : 0));
	float rotY = ((Keys::down.right ? +1.f : 0) - (Keys::down.left ? +1.f : 0));
	static float rotAngX = 0, rotAngY = 0;
	rotAngX = rotX ? clamp(-1.f,+1.f, rotAngX+rotX*dt*5) : 0;
	rotAngY = rotY ? clamp(-1.f,+1.f, rotAngY+rotY*dt*5) : 0;
	motion.axes = motion.axes.out(rotateX(UnitAxes, rotAngX * dt * 2));
	Matrix result = matrix.out(motion);
	result.axes = rotateY(result.axes, rotAngY * dt * 2);
	return result;
}
