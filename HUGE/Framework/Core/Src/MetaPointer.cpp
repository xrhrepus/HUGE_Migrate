#include "Precompiled.h"
#include "MetaPointer.h"

H::Core::Meta::MetaPointer::MetaPointer(const MetaType * ptrType)
	:MetaType(MetaType::Category::Pointer,"Pointer",sizeof(nullptr),nullptr,nullptr),mPointerType(ptrType)
{
}
