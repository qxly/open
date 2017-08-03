#include <open/Memory.h>
#if USE_TC_MEMORY
#include <tcmalloc/tcmalloc.h>
#pragma comment(lib, "libtcmalloc_minimal-debug.lib")

namespace open
{
	void* MemoryAlloc::malloc(size_t size)
	{
		return tc_malloc(size);
	}

	void MemoryAlloc::free(void* p)
	{
		tc_free(p);
	}
}

#else
namespace open
{
	void* MemoryAlloc::malloc(size_t size)
	{
		return ::malloc(size);
	}

	void MemoryAlloc::free(void* p)
	{
		::free(p);
	}

	void* MemoryAlloc::align_malloc(size_t size, size_t alignment)
	{
		return _aligned_malloc(size, alignment);
	}

	void MemoryAlloc::align_free(void* p)
	{
		_aligned_free(p);
	}
}

#endif

#ifdef _DEBUG
int64 open::Object_Debug::s_objectCount = 0;
#endif