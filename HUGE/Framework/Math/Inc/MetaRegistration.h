#ifndef HUGE_MATH_METAREGISTRATION_H
#define HUGE_MATH_METAREGISTRATION_H

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

META_TYPE_DECLARE(H::Math::Vector2)
META_TYPE_DECLARE(H::Math::Vector3)
META_TYPE_DECLARE(H::Math::Vector4)
META_TYPE_DECLARE(H::Math::Quaternion)
META_TYPE_DECLARE(H::Math::Matrix4)


namespace H::Math
{
	void StaticMetaRegistration();


}


#endif // !HUGE_MATH_METAREGISTRATION_H

