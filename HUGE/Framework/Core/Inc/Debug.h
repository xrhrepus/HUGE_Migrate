#ifndef HUGE_CORE_DEBUG_H
#define HUGE_CORE_DEBUG_H
// ... -> __VA_ARGS__
//do-while false -> keep code copy in one  { }

//macro substuting code
#if defined(_DEBUG)

#define LOG(format, ...)\
	do {\
		char buffer[4096];\
		int ret = _snprintf_s(buffer,std::size(buffer),_TRUNCATE, "%s(%d)"##format, __FILE__,__LINE__,__VA_ARGS__);\
		OutputDebugStringA(buffer);\
		if (ret == -1)\
			OutputDebugStringA("** message truncated ** \n");\
		OutputDebugStringA("\n");\
	}while (false)

	#define ASSERT(condition, format, ...)\
		do{\
			if(!(condition))\
			{\
				LOG(format, __VA_ARGS__);\
				DebugBreak();\
			}\
		}while (false)
#else
	#define LOG(format, ...)
	#define ASSERT(condition, format, ...)
#endif

#endif // !HUGE_CORE_DEBUG_H
