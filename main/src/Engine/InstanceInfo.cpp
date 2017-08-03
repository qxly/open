#include "InstanceInfo.h"

namespace open
{
	int32 GeneralInstance::instancesElementSize()
	{
		return 8;
	}

	const ElementInfo& GeneralInstance::elementFloatSize()
	{
		static const ElementInfo s_elementFloatSize = { 4, 4, 4, 4, 4, 4, 4, 4 };
		return s_elementFloatSize;
	}

	const ElementInfo& GeneralInstance::offset()
	{
		static const ElementInfo s_offset = { 0, 16, 16 * 2, 16 * 3, 16 * 4, 16 * 5, 16 * 6, 16 * 7 };
		return s_offset;
	}

	const ElementInfo& GeneralInstance::stride()
	{
		static const ElementInfo s_stride = { sizeof(GeneralInstance), sizeof(GeneralInstance), sizeof(GeneralInstance), sizeof(GeneralInstance),
			sizeof(GeneralInstance), sizeof(GeneralInstance), sizeof(GeneralInstance), sizeof(GeneralInstance) };

		return s_stride;
	}
}