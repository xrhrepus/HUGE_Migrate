#ifndef HUGE_CORE_METAFIELD_H
#define HUGE_CORE_METAFIELD_H

namespace H::Core::Meta
{
	class MetaType;

	class MetaField
	{
	public:
		MetaField(const MetaType* type, const char* name, size_t offset);
		
		const MetaType* GetMetaType() const { return mType; }
		const char* GetName() const { return mName.c_str(); }
		size_t GetOffset() const { return mOffset; }

	private:
		const std::string mName;
		const MetaType* mType;
		const size_t mOffset;

	};

 

}//namespace H::Core::Meta


#endif // !HUGE_CORE_METAFIELD_H

