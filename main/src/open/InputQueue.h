#ifndef INPUT_GUIEVENTQUEUE_H
#define INPUT_GUIEVENTQUEUE_H 1

#include <open/InputInterface.h>
#include <open/IntrusiveList.h>

namespace open
{
	struct InputLink
	{
		Input e;
		TLink<InputLink> link;

		InputLink(const Input& ev)
			:e(ev){}
	};

	class InputQueue : public IInputQueue
	{
	public:

		InputQueue();

		virtual const Input* nextEvent();

		virtual void endEvent();

		virtual void pauseEvent();

		////Êó±ê
		virtual void mouseMotion(int32 x, int32 y);

		virtual void mouseButtonPress(int32 x, int32 y, uint32 button);

		virtual void mouseDoubleButtonPress(int32 x, int32 y, uint32 button) ;

		virtual void mouseButtonRelease(int32 x, int32 y, uint32 button);

		virtual void mouseWheel(int32 x, int32 y, int32 wheel);

		///¼üÅÌ
		virtual void keyPress(int key);

		virtual void keyRelease(int key);

		///´°¿Ú
		virtual void changeWindow(int32 windowX,int32 windowY,int32 width,int32 height);

		virtual void closeWindow();

	private:
	
		void addInput(const Input& event);

	private:
		bool _lastPush;
		Input _accumulateEventState;

		InputLink* _currentEventLink;

		INTRUSIVELIST_DECLARE(InputLink,link) _guiEvents;
	};
}

#endif