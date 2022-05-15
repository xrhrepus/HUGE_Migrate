#ifndef HUGE_CORE_METAREGISTRATION_H
#define HUGE_CORE_METAREGISTRATION_H
#include "Meta.h"
namespace H::Core
{
//primitive type declaration

	META_TYPE_DECLARE(int)
	META_TYPE_DECLARE(float)
	META_TYPE_DECLARE(bool)
	META_TYPE_DECLARE(std::string)

	void StaticMetaRegistration();
}

#endif // !HUGE_CORE_METAREGISTRATION_H


