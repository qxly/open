//#ifndef WYIN_MULITICOMMAND_H
//#define WYIN_MULITICOMMAND_H 1
//
//#include <open/Deque.h>
//#include <openDef.h>
//
//class CommandProcess
//{
//public:
//	virtual void process(int id,void* data,bool discarded) = 0;
//};
//
//#define MAX_COMMAND_TYPE_NUM 64
//class CommandGroup
//{
//private:
//	typedef Deque<void*> CommandList;
//public:
//
//	CommandGroup(int maxCommandTypeSize)
//		:_maxCommandTypeSize(maxCommandTypeSize)
//	{
//		_command = new CommandList[_maxCommandTypeSize];
//	}
//
//	~CommandGroup()
//	{
//		delete [] _command;
//	}
//
//	inline void setCommand(int id,void* data)
//	{
//		_mutex.lock();
//
//		_command[id].push_back(data);
//
//		_mutex.unlock();
//	}
//
//	void process(CommandProcess& process)
//	{
//		CommandList temps[MAX_COMMAND_TYPE_NUM];
//
//		_mutex.lock();
//		for(int i = 0; i < _maxCommandTypeSize; i++)
//		{
//			temps[i] = _command[i];
//		}
//		_mutex.unlock();
//
//		for(int i = 0; i < _maxCommandTypeSize; i++)
//		{
//			CommandList& commands = _command[i]; 
//			if(commands.empty())
//				continue;
//			process.process(i,commands.back(),false);
//
//			while(!commands.empty())
//			{
//				process.process(i,commands.front(),true);
//				commands.pop_front();
//			}
//		}
//	}
//
//	void discard(CommandProcess& process)
//	{
//		for(int i = 0; i < _maxCommandTypeSize; i++)
//		{
//			CommandList& commands = _command[i]; 
//			if(commands.empty())
//				continue;
//
//			while(!commands.empty())
//			{
//				process.process(i,commands.front(),false);
//				commands.pop_front();
//			}
//		}
//	}
//
//private:
//
//	int _maxCommandTypeSize;
//	CommandList* _command;
//
//	boost::mutex _mutex;
//};
//
//#endif