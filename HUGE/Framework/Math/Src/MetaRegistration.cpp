//MetaRegistration
#include "Precompiled.h"
#include "MetaRegistration.h"
namespace H::Core::Meta
{
	using namespace H::Math;
	template<>
	void Deserialize<Vector2>(void* instance, const rapidjson::Value& jsonValue)
	{
		auto object = jsonValue.GetObjectW();
		auto vec2 = (Vector2*)(instance);
		vec2->x = object.FindMember("x")->value.GetFloat();
		vec2->y = object.FindMember("y")->value.GetFloat();
	}
	template<>
	void Deserialize<Vector3>(void* instance, const rapidjson::Value& jsonValue)
	{
		auto object = jsonValue.GetObjectW();
		auto vec3 = (Vector3*)(instance);
		vec3->x = object.FindMember("x")->value.GetFloat();
		vec3->y = object.FindMember("y")->value.GetFloat();
		vec3->z = object.FindMember("z")->value.GetFloat();

	}
	template<>
	void Deserialize<Vector4>(void* instance, const rapidjson::Value& jsonValue)
	{
		auto object = jsonValue.GetObjectW();
		auto vec4 = (Vector4*)(instance);
		vec4->x = object.FindMember("x")->value.GetFloat();
		vec4->y = object.FindMember("y")->value.GetFloat();
		vec4->z = object.FindMember("z")->value.GetFloat();
		vec4->w = object.FindMember("w")->value.GetFloat();

	}
	template<>
	void Deserialize<Quaternion>(void* instance, const rapidjson::Value& jsonValue)
	{
		auto object = jsonValue.GetObjectW();
		auto quat = (Quaternion*)(instance);
		quat->x = object.FindMember("x")->value.GetFloat();
		quat->y = object.FindMember("y")->value.GetFloat();
		quat->z = object.FindMember("z")->value.GetFloat();
		quat->w = object.FindMember("w")->value.GetFloat();

	}
}

META_TYPE_DEFINE(H::Math::Vector2,Vector2)
META_TYPE_DEFINE(H::Math::Vector3,Vector3)
META_TYPE_DEFINE(H::Math::Vector4,Vector4)
META_TYPE_DEFINE(H::Math::Quaternion,Quaternion)
//META_TYPE_DEFINE(H::Math::Matrix4,Matrix4)

void H::Math::StaticMetaRegistration()
{

}