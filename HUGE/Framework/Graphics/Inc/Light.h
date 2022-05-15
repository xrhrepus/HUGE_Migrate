#ifndef HUGE_GRAPHICS_LIGHT_H
#define HUGE_GRAPHICS_LIGHT_H

#include "Colors.h"

namespace H::Graphics
{
	struct PointLight
	{
		Math::Vector3 position;
		float padding;
		Color ambient;
		Color diffuse;
		Color specular;


	};
	struct DirectionalLight
	{
		Math::Vector3 direction{ 1.0f,0.0f,0.0f };
		float padding;
		Color ambient{ 1.0f,1.0f, 1.0f, 1.0f };
		Color diffuse{ 1.0f,1.0f, 1.0f, 1.0f };
		Color specular{ 1.0f,1.0f, 1.0f, 1.0f };
	};

}//namespace H::Graphics



#endif // !HUGE_GRAPHICS_LIGHT_H
