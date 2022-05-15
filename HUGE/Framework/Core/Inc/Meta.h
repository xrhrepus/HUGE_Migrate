#ifndef HUGE_CORE_META_H
#define HUGE_CORE_META_H

#include "MetaArray.h"
#include "MetaClass.h"
#include "MetaField.h"
#include "MetaPointer.h"
#include "MetaRegistry.h"
#include "MetaType.h"
#include "MetaUtil.h"

namespace H::Core
{
	
#define	META_TYPE_DECLARE(Type)\
	template<> const H::Core::Meta::MetaType* H::Core::Meta::DeduceType<Type>();

#define	META_TYPE_DEFINE(Type,Name)\
	template<> const H::Core::Meta::MetaType* H::Core::Meta::DeduceType<Type>()\
	{\
		static const H::Core::Meta::MetaType sMetaType(\
			H::Core::Meta::MetaType::Category::Primitive, #Name, sizeof(Type),\
		[] {return new Type; },\
		[](void* data) { delete static_cast<Type*>(data); },\
		nullptr,\
		H::Core::Meta::Deserialize<Type>);\
		return &sMetaType;\
	}

#define META_CLASS_DECLARE\
	static const H::Core::Meta::MetaClass* StaticGetMetaClass();\
	virtual const H::Core::Meta::MetaClass* GetMetaClass() const {return StaticGetMetaClass(); }

#define META_DERIVED_BEGIN(ClassType, ParentType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const H::Core::Meta::MetaClass* parentMetaClass = ParentType::StaticGetMetaClass();

#define META_CLASS_BEGIN(ClassType)\
	META_CLASS_BEGIN_INTERNAL(ClassType)\
		const H::Core::Meta::MetaClass* parentMetaClass = nullptr;

#define META_CLASS_BEGIN_INTERNAL(ClassType)\
	template <> const H::Core::Meta::MetaType* H::Core::Meta::DeduceType<ClassType>()\
	{\
		return ClassType::StaticGetMetaClass();\
	}\
	const H::Core::Meta::MetaClass* ClassType::StaticGetMetaClass()\
	{\
		using Class = ClassType;\
		const char* className = #ClassType;

#define META_FIELD_BEGIN\
		static const std::initializer_list<H::Core::Meta::MetaField> fields {

#define META_FIELD(Var, Name)\
			{ H::Core::Meta::DeduceMemberType(&Class::Var), Name, H::Core::Meta::GetFieldOffset(&Class::Var) },

#define META_FIELD_END\
		};

#define META_NO_FIELD\
		static const std::initializer_list<H::Core::Meta::MetaField> fields;

#define META_CLASS_END\
		static const H::Core::Meta::MetaClass sMetaClass(\
			className, sizeof(Class),\
			[] { return new Class(); },\
			[](void* data) { delete static_cast<Class*>(data); },\
			parentMetaClass, fields);\
		return &sMetaClass;\
	}






}//namespace H::Core

#endif // !HUGE_CORE_META_H
