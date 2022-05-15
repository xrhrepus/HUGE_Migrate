#include "Precompiled.h"
#include "MetaArray.h"

H::Core::Meta::MetaArray::MetaArray(const MetaType * elementType, GetElementFunc getElement, GetCountFunc getcount)
	:MetaType(MetaType::Category::Array,"Array",sizeof(std::vector<int>),nullptr,nullptr),
	mElementType(elementType),
	mGetElement(std::move( getElement)),
	mGetCount(std::move(getcount))
{
}
