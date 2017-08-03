#ifndef RENDERSYSTEM_GPURESOURCE_H
#define RENDERSYSTEM_GPURESOURCE_H

#include <open/IntrusiveList.h>

namespace open
{
	class IGPUResource
	{
	public:
		IGPUResource();

		virtual ~IGPUResource();

		virtual void releaseGPUResource() = 0;

	public:

		TLink<IGPUResource> resourceLink;
	};

	class GPUResourceManager
	{
		typedef IntrusiveListDeclare<IGPUResource, offsetof(IGPUResource, resourceLink)> GPUResourceList;
		GPUResourceList _list;

	public:

		GPUResourceManager* s_manager;

	public:
		inline void create(IGPUResource* resouce)
		{
			_list.push_back(resouce);
		}
	};
}
#endif