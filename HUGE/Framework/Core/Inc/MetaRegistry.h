#ifndef HUGE_CORE_META_METAREGISTRY_H
#define HUGE_CORE_META_METAREGISTRY_H

namespace H::Core::Meta
{
	class MetaClass;

	void Register(const MetaClass* metaClass);

	const MetaClass* FindMetaClass(const std::string& className);
}

#define META_REGISTRY(Class)\
	H::Core::Meta::Register(Class::StaticGetMetaClass())


#endif // !HUGE_CORE_META_METAREGISTRY_H

