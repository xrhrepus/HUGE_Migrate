#pragma once
//#include "Math/Inc/BoundingVolumn.h"
#include <HUGE/Inc/HUGE.h>
class TRaycast
{
public:
	void Init();
	Ray CastRay(int mouseScreenX, int mouseScreenY,float farDistance,const Vector3& camPos,const Matrix4& invVP);
	Vector3 ConvertScreenToNDC(int mouseScreenX, int mouseScreenY, float farDistance, const Matrix4& invVP);
	void DebugUI();
private:
	float step_w;
	float step_h;

	Vector3 _ndcV;
	Vector3 _tfV;

	Vector3 normal_tfV;

	Vector3 posWorld;
};

 