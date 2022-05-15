#ifndef HUGE_CORE_META_METACLASS_H
#define HUGE_CORE_META_METACLASS_H
#include "MetaType.h"

namespace H::Core::Meta
{

	class MetaField;

	class MetaClass : public MetaType
	{
	public:
		MetaClass(const char* name, size_t size,
			MetaType::CreateFunc create,MetaType::DestoryFunc destory,
			const MetaClass* parent,const std::vector< MetaField> fields);

		const MetaClass* GetParent() const;
		const MetaField* FindField(const char* name)const;
		const MetaField* GetField(size_t index)const;
		size_t GetFieldsCount()const;

	public:

		void Serialize(const void* instance, rapidjson::Value& jsonValue) const override;
		void Deserialize(void* instance, const rapidjson::Value& jsonValue) const override;


	private:
		size_t GetParentFieldsCount() const;
		const MetaClass* mParent;
		const std::vector< MetaField> mFields;

	};

}
#endif // !HUGE_CORE_METACLASS_H
 
  