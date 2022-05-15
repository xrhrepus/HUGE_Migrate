#ifndef HUGE_CORE_META_METATYPE_H
#define HUGE_CORE_META_METATYPE_H
#include "Debug.h"


namespace H::Core::Meta
{
	class MetaArray;
	class MetaPointer;
	class MetaClass;

	class MetaType
	{

	public:
		using CreateFunc = std::function<void*()>;
		using DestoryFunc = std::function<void(void*)>;
		using SerializeFunc = std::function<void(const void* instance , rapidjson::Value& jsonValue)>;
		using DeserializeFunc = std::function<void(void* instance , const rapidjson::Value& jsonValue)>;

		enum class Category
		{
			Primitive,
			Class,
			Array,
			Pointer
		};

		MetaType(Category category, 
			const char* name,
			size_t size,
			CreateFunc create,DestoryFunc destory,
			SerializeFunc serialize = nullptr,
			DeserializeFunc deserialize = nullptr);
		
		const char* GetName() const { return mName.c_str(); }
		Category GetCategory() const { return mCategory; }
		size_t GetSize() const { return mSize; }

		void* Create() const;
		void Destory(void* data) const;
		virtual void Serialize(const void* instance, rapidjson::Value& jsonValue) const;
		virtual void Deserialize(void* instance, const rapidjson::Value& jsonValue) const;

		//const MetaArray* GetMetaArray()const;
		//const MetaClass* GetMetaClass()const;
		//const MetaPointer* GetMetaPointer()const;


	private:
		const std::string mName;
		const Category mCategory;
		const size_t mSize;

		const CreateFunc mCreate;
		const DestoryFunc mDestory;
		const SerializeFunc mSerialize;
		const DeserializeFunc mDeserialize;
	};
}

 
#endif // !HUGE_CORE_METATYPE_H



