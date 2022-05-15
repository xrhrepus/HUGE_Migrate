#include "Precompiled.h"
#include "MetaField.h"
#include "MetaClass.h"
using namespace H::Core::Meta;

namespace 
{
 
}

H::Core::Meta::MetaClass::MetaClass(const char * name, size_t size, MetaType::CreateFunc create, MetaType::DestoryFunc destory, const MetaClass * parent, const std::vector<MetaField> fields)
	:MetaType(MetaType::Category::Class,name,size,std::move(create),std::move(destory))
	,mParent(parent)
	,mFields(std::move(fields))
{
}

const MetaClass * H::Core::Meta::MetaClass::GetParent() const
{
	return mParent;
}

const MetaField * H::Core::Meta::MetaClass::FindField(const char * name) const
{
	for (auto& field : mFields)
	{
		if (strcmp(field.GetName(),name) == 0)
		{
			return &field;
		}
	}

	if (mParent!=nullptr)
	{
		return mParent->FindField(name);
	}

	return nullptr;
}

const MetaField * H::Core::Meta::MetaClass::GetField(size_t index) const
{
	ASSERT(index < GetFieldsCount(), "[MetaClass] Subscript out of range");
	const size_t parentCount = GetParentFieldsCount();

	if (index < parentCount)
	{
		return mParent->GetField(index);
	}

	return mFields.data() + (index - parentCount);
}

size_t H::Core::Meta::MetaClass::GetFieldsCount() const
{
	return mFields.size() + GetParentFieldsCount();
}

void H::Core::Meta::MetaClass::Serialize(const void * instance, rapidjson::Value & jsonValue) const
{
	ASSERT(false, "[MetaType] todo");

}

void H::Core::Meta::MetaClass::Deserialize(void * instance, const rapidjson::Value & jsonValue) const
{
	for (auto& member : jsonValue.GetObjectW())
	{
		auto metaField = FindField(member.name.GetString());
		auto metaType = metaField->GetMetaType();
		void * instanceField = static_cast<uint8_t*>(instance) + metaField->GetOffset();
		metaType->Deserialize(instanceField, member.value);
 
	}

}

size_t H::Core::Meta::MetaClass::GetParentFieldsCount() const
{
	return mParent ? mParent->GetFieldsCount() : 0u;
}
