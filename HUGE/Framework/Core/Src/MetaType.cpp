#include "Precompiled.h"
#include "MetaType.h"
#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaPointer.h"
using namespace H::Core::Meta;
 
H::Core::Meta::MetaType::MetaType(Category category, const char * name, size_t size,
	CreateFunc create, DestoryFunc destory, SerializeFunc serialize,DeserializeFunc deserialize)
	:mName(name),mCategory(category),mSize(size),
	mCreate(std::move( create)),mDestory(std::move(destory)),
	mSerialize(std::move(serialize)),mDeserialize(std::move(deserialize))
{
}


void* H::Core::Meta::MetaType::Create() const
{
	ASSERT(mCreate, "[MetaType] no creation callable registered for '%s'", GetName());
	return mCreate();
}

void H::Core::Meta::MetaType::Destory(void* data) const
{
	ASSERT(mDestory, "[MetaType] no destruction callable registered for '%s'", GetName());
	mDestory(data);
}
void H::Core::Meta::MetaType::Serialize(const void* instance, rapidjson::Value& jsonValue) const
{
	ASSERT(mSerialize, "[MetaType] no Serialize callable registered for '%s'", GetName());
	mSerialize(instance, jsonValue);
}
void H::Core::Meta::MetaType::Deserialize(void* instance, const rapidjson::Value& jsonValue) const
{
	ASSERT(mDeserialize, "[MetaType] no Deserialize callable registered for '%s'", GetName());
	mDeserialize(instance, jsonValue);
}
//const H::Core::Meta::MetaArray * H::Core::Meta::MetaType::GetMetaArray() const
//{
//	ASSERT(mCategory == Category::Array, "[MetaType] Invalid type cast");
//	return static_cast<const H::Core::Meta::MetaArray *>(this);
//}
//
//const H::Core::Meta::MetaClass * H::Core::Meta::MetaType::GetMetaClass() const
//{
//	ASSERT(mCategory == Category::Class, "[MetaType] Invalid type cast");
//	return static_cast<const H::Core::Meta::MetaClass *>(this);
//}
//
//const H::Core::Meta::MetaPointer * H::Core::Meta::MetaType::GetMetaPointer() const
//{
//	ASSERT(mCategory == Category::Pointer, "[MetaType] Invalid type cast");
//	return static_cast<const H::Core::Meta::MetaPointer *>(this);
//}
