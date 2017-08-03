#ifndef RENDERSYSTEM_DEF_H
#define RENDERSYSTEM_DEF_H

#ifdef _WIN32
#ifdef RENDERSYSTEM_EXPORTS
#    define RENDERSYSTEM_API __declspec(dllexport)
#else
#    define RENDERSYSTEM_API __declspec(dllimport)
#endif
#else
#define RENDERSYSTEM_API
#endif

#include <open/openDef.h>
#include <open/Memory.h>

#endif