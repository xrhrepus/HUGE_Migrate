//
#ifndef HUGE_CORE_H
#define HUGE_CORE_H

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "Common.h"

#include "Debug.h"
#include "Window.h"
#include "WindowMessageHandler.h"
#include "BlockAllocator.h"
#include "TypedAllocator.h"
#include "Handle.h"
#include "HandlePool.h"
#include "Meta.h"
#include "MetaRegistration.h"



//#include "Math/Inc/HUGEMath.h"


//utility function to release D3D interfaces
template<class T>
inline void SafeRelease(T*& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

#endif // !INCLUDED_HUGE_CORE_H

