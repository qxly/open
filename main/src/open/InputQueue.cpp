#include "InputQueue.h"

namespace open
{
	IInputQueue* createInputQueue() { return new InputQueue;}

	InputQueue::InputQueue()
		:_lastPush(false),_currentEventLink(nullptr)
	{
	}

	const Input* InputQueue::nextEvent()
	{
		if(!_currentEventLink)
			_currentEventLink = _guiEvents.front();
		else
		{
			_currentEventLink = _guiEvents.next(_currentEventLink);
		}
		if(_currentEventLink)
			return (&_currentEventLink->e);
		else
			return nullptr;
	}

	void InputQueue::endEvent()
	{
		_currentEventLink = nullptr;
		_guiEvents.clear_destory();
	}

	void InputQueue::pauseEvent()
	{
		_currentEventLink = nullptr;
	}

	void InputQueue::addInput(const Input& event)
	{
		InputLink* link = new InputLink(event);
		_guiEvents.push_back(link);
	}

	////ЪѓБъ
	void InputQueue::mouseMotion(int32 x, int32 y)
	{
		_accumulateEventState.setX(x);
		_accumulateEventState.setY(y);

		Input event(_accumulateEventState);
		event.setEventType(_lastPush ? DRAG : MOVE);

		addInput(event);
	}

	void InputQueue::mouseButtonPress(int32 x, int32 y, uint32 button)
	{
		_accumulateEventState.setX(x);
		_accumulateEventState.setY(y);

		switch(button)
		{
		case(1): 
			_accumulateEventState.setMouseButton(LEFT_MOUSE_BUTTON);
			break;
		case(2): 
			_accumulateEventState.setMouseButton(MIDDLE_MOUSE_BUTTON);
			break;
		case(3): 
			_accumulateEventState.setMouseButton(RIGHT_MOUSE_BUTTON);
			break;
		}

		Input event(_accumulateEventState);
		event.setEventType(PUSH);

		_lastPush = true;
		addInput(event);
	}

	void InputQueue::mouseDoubleButtonPress(int32 x, int32 y, uint32 button)
	{
		_accumulateEventState.setX(x);
		_accumulateEventState.setY(y);

		switch(button)
		{
		case(1): 
			_accumulateEventState.setMouseButton(LEFT_MOUSE_BUTTON);
			break;
		case(2): 
			_accumulateEventState.setMouseButton(MIDDLE_MOUSE_BUTTON);
			break;
		case(3): 
			_accumulateEventState.setMouseButton(RIGHT_MOUSE_BUTTON);
			break;
		}

		Input event(_accumulateEventState);
		event.setEventType(DOUBLECLICK);

		_lastPush = false;
		addInput(event);
	}

	void InputQueue::mouseButtonRelease(int32 x, int32 y, uint32 button)
	{
		_accumulateEventState.setX(x);
		_accumulateEventState.setY(y);

		switch(button)
		{
		case(1): 
			_accumulateEventState.setMouseButton(LEFT_MOUSE_BUTTON);
			break;
		case(2): 
			_accumulateEventState.setMouseButton(MIDDLE_MOUSE_BUTTON);
			break;
		case(3): 
			_accumulateEventState.setMouseButton(RIGHT_MOUSE_BUTTON);
			break;
		}

		Input event(_accumulateEventState);
		event.setEventType(RELEASE);

		_lastPush = false;
		addInput(event);
	}

	void InputQueue::mouseWheel(int32 x, int32 y, int32 wheel)
	{
	}

	///МќХЬ
	void InputQueue::keyPress(int key)
	{
		_accumulateEventState.setKey(key);

		Input event(_accumulateEventState);
		event.setEventType(KEYDOWN);

		addInput(event);
	}

	void InputQueue::keyRelease(int key)
	{
		_accumulateEventState.setKey(key);

		Input event(_accumulateEventState);
		event.setEventType(KEYUP);

		addInput(event);
	}

	void InputQueue::changeWindow(int32 windowX,int32 windowY,int32 width,int32 height)
	{
		_accumulateEventState.setWindowX(windowX);
		_accumulateEventState.setWindowY(windowY);
		_accumulateEventState.setWindowWidth(width);
		_accumulateEventState.setWindowHeight(height);

		Input event(_accumulateEventState);
		event.setEventType(RESIZE);

		_lastPush = false;
		addInput(event);
	}

	void InputQueue::closeWindow()
	{
		Input event;
		event.setEventType(CLOSE);
		addInput(event);
	}
}