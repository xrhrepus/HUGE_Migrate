#include "Precompiled.h"
#include "MetaField.h"
 

H::Core::Meta::MetaField::MetaField(const MetaType * type, const char * name, size_t offset)
	:mType(type),mName(name),mOffset(offset)
{
}
