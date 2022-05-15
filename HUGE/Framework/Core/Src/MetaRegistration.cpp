#include "Precompiled.h"
#include "MetaRegistration.h"
#include <RapidJSON/Inc/document.h>
using namespace H::Core;
namespace H::Core::Meta
{

	template<>
	void Deserialize<int>(void* instance, const rapidjson::Value& jsonValue)
	{
		*(int*)(instance) = jsonValue.GetInt();
	}
	template<>
	void Deserialize<float>(void* instance, const rapidjson::Value& jsonValue)
	{
		*(float*)(instance) = jsonValue.GetFloat();
	}
	template<>
	void Deserialize<bool>(void* instance, const rapidjson::Value& jsonValue)
	{
		*(bool*)(instance) = jsonValue.GetBool();
	}
	template<>
	void Deserialize<std::string>(void* instance, const rapidjson::Value& jsonValue)
	{
		*(std::string*)(instance) = jsonValue.GetString();
	}
}

META_TYPE_DEFINE(int, Integer)
META_TYPE_DEFINE(float, Number)
META_TYPE_DEFINE(bool, Boolean)
META_TYPE_DEFINE(std::string, String)


void H::Core::StaticMetaRegistration()
{

}
