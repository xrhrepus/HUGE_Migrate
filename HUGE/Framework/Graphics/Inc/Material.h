#ifndef HUGE_GRAPHICS_MATERIAL_H
#define HUGE_GRAPHICS_MATERIAL_H

#include "Colors.h"

namespace H::Graphics
{

	struct Material
	{
		Color ambient{ 1.0f,1.0f, 1.0f, 1.0f };
		Color diffuse{ 1.0f,1.0f, 1.0f, 1.0f };
		Color specular{ 1.0f,1.0f, 1.0f, 1.0f };
		float power{1.0f};
		float padding[3];
	};


}



#endif //!HUGE_GRAPHICS_MATERIAL_H