#ifndef ENGINE_INSTANCE_INFO_H
#define ENGINE_INSTANCE_INFO_H

#include <common/Maths.h>
#include <common/Array.h>
#include <common/interface/RenderSystem/RenderEnumeration.h>

namespace open
{
	typedef int32 ElementInfo[VERTEX_NULL - VERXT_INSTANCE1];

	struct GeneralInstance
	{
		RMatrix matrix;
		RMatrix normalMatrix;

		virtual const float* ptr() const { return matrix.ptr(); }

		static int32 floatSize() { return 32; }

		static int32 instancesElementSize();

		static const ElementInfo& elementFloatSize();

		static const ElementInfo& offset();

		static const ElementInfo& stride();
	};
	DECLARE_AND_TYPEDEF_SAMLLTYPE_ARRARY(GeneralInstance);
}

#endif