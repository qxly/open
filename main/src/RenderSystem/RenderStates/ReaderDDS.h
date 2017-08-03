#ifndef RENDERSYSTEM_READDDS_H
#define RENDERSYSTEM_READDDS_H 1

#include <open/openDef.h>

namespace open
{
	class ITexture;
	ITexture* ReadDDSFile(char* ddsBuffer,int32 bufferSize);
}

#endif