#ifndef HUGE_CORE_META_METAARRAY_H
#define HUGE_CORE_META_METAARRAY_H
#include "MetaType.h"
namespace H::Core::Meta
{

	class MetaArray : public MetaType
	{
	public:
		using GetElementFunc = std::function<void*(void*, size_t)>;
		using GetCountFunc = std::function<size_t(void*)>;

		MetaArray(const MetaType* elementType,GetElementFunc getElement, GetCountFunc getcount);
 
		const MetaType* GetElementType()const { return mElementType; }
		void* GetElement(void* instance, size_t index)const { return mGetElement(instance, index); }
		size_t GetCount(void* instance)const { return mGetCount(instance); }
	private:
		const MetaType* mElementType;
		GetElementFunc mGetElement;
		GetCountFunc mGetCount;
	};

}//namespace H::Core::Meta

  
#endif // !HUGE_CORE_META_METAARRAY_H

