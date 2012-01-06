#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <glog/logging.h>

class EventManager
{
	public:
		typedef std::map<std::string, boost::any> Arguments;
		
		template<class TClass>
		static int subscribe(std::string event_name, 
								TClass* _ptObject, 
								void (TClass::* _ptFunc)(const std::string, const std::map<std::string, boost::any>)) {
			LOG(INFO) << "Subscribing to event: " << event_name;
			struct Callback cbk;
			cbk.id = id_count;
			cbk.method = boost::bind(_ptFunc, _ptObject, _1, _2);
			registered_cbks.insert(std::pair<std::string, Callback>(event_name,cbk));
			++id_count;
			return cbk.id;
		}
		static bool unSubscribe(const int);
		static void sendEvent(std::string, Arguments=Arguments());
		static void sendEventSync(std::string, Arguments=Arguments());

		static void processEvents();
		static void listListeners();
		
	private:
		EventManager();
		~EventManager(){};
	
		typedef boost::function<void (const std::string, const std::map<std::string, boost::any>)> CallBkFunc;
		struct Callback {
			int id;
			CallBkFunc method;
		};
		typedef std::multimap<std::string, Callback> ListRegisteredCbks;
		
		static ListRegisteredCbks registered_cbks;
		static int id_count;
		struct MyEvent {
			std::string event_name;
			Arguments args;
		};
		static std::queue<MyEvent> event_queue;
};

#endif
