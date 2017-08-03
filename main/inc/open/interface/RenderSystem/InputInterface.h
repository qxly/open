#ifndef RENDERSYSTEM_INPUTINTERFACE_H
#define RENDERSYSTEM_INPUTINTERFACE_H

#include <RenderSystem/RenderSystemDef.h>
#include <RenderSystem/RenderEnumeration.h>

namespace wyin
{
	struct Input
	{
		InputType eventType;

		int32 key;

		uint32 buttonMask;
		int32 mx;
		int32 my;

		int32 windowX;
		int32 windowY;
		int32 windowWidth;
		int32 windowHeight;

		inline void setEventType(InputType Type) { eventType = Type; }

		inline void setKey(int32 k) { key = k; }

		inline void setX(int32 x) { mx = x; }
		inline void setY(int32 y) { my = y; }

		inline void setMouseButton(uint32 mask) { buttonMask = mask; }

		inline void setWindowX(int32 x) { windowX = x;}
		inline void setWindowY(int32 y) { windowY = y;}
		inline void setWindowWidth(int32 widht) { windowWidth = widht;}
		inline void setWindowHeight(int32 height) { windowHeight = height;}
	};

	class RENDERSYSTEM_API IInputQueue
	{
	public:
		virtual const Input* nextEvent() = 0;

		virtual void endEvent() = 0;

		////Êó±ê
		virtual void mouseMotion(int32 x, int32 y) = 0;

		virtual void mouseButtonPress(int32 x, int32 y, uint32 button) = 0;

		virtual void mouseDoubleButtonPress(int32 x, int32 y, uint32 button) = 0;

		virtual void mouseButtonRelease(int32 x, int32 y, uint32 button) = 0;

		///¼üÅÌ
		virtual void keyPress(int key) = 0;

		virtual void keyRelease(int key) = 0;

		///´°¿Ú
		virtual void changeWindow(int32 windowX,int32 windowY,int32 width,int32 height) = 0;
	};
}

#endif