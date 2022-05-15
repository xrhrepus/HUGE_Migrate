#ifndef HUGE_CORE_META_METAPOINTER_H
#define HUGE_CORE_META_METAPOINTER_H
#include "MetaType.h"

namespace H::Core::Meta
{
 
	class MetaPointer : public MetaType
	{
	public:
		MetaPointer(const MetaType* ptrType);
		
		const MetaType* GetPointerType() const { return mPointerType; }

	private:
		const MetaType* mPointerType;
	};

 


}//namespace H::Core::Meta

#endif // !HUGE_CORE_META_METAPOINTER_H

