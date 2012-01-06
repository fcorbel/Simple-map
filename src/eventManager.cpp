#include "eventManager.h"

EventManager::ListRegisteredCbks EventManager::registered_cbks = ListRegisteredCbks();
int EventManager::id_count = 0;
std::queue<EventManager::MyEvent> EventManager::event_queue = std::queue<EventManager::MyEvent>();

bool EventManager::unSubscribe(int id)
{
	//not perf efficient
	std::multimap<std::string, Callback>::iterator it;
	for (it = registered_cbks.begin(); it != registered_cbks.end(); ++it) {
			LOG(INFO) << it->second.id;
		if (it->second.id == id) {
				registered_cbks.erase(it);
		}
	}
 
	return true;
}

void EventManager::sendEvent(std::string event_name, Arguments args)
{
	//LOG(INFO) << "received event: " << event_name;
	MyEvent ev;
	ev.event_name = event_name;
	ev.args = args; 
	event_queue.push(ev);
}

void EventManager::sendEventSync(std::string event_name, Arguments args)
{
		std::pair<ListRegisteredCbks::iterator, ListRegisteredCbks::iterator> it;
		it = registered_cbks.equal_range(event_name);
		for (ListRegisteredCbks::iterator it2 = it.first; it2 != it.second; ++it2) {
			LOG(INFO) << "Call a matching method synchronously for event: " << "\"" << event_name << "\"";
			it2->second.method(it2->first, args);
		}
}

void EventManager::processEvents ()
{
	while (!event_queue.empty()) {
		LOG(INFO) << "Processing some events in queue";
		std::pair<ListRegisteredCbks::iterator, ListRegisteredCbks::iterator> it;
		it = registered_cbks.equal_range(event_queue.front().event_name);
		for (ListRegisteredCbks::iterator it2 = it.first; it2 != it.second; ++it2) {
			LOG(INFO) << "Call a matching method for event: " << "\"" << event_queue.front().event_name << "\"";
			it2->second.method(it2->first, event_queue.front().args);
		}
		event_queue.pop();
	}
	//~ LOG(INFO) << "All events processed";
}

void EventManager::listListeners()
{
	if (!registered_cbks.empty()) {
		ListRegisteredCbks::iterator it;
		for (it = registered_cbks.begin(); it != registered_cbks.end(); ++it) {
			std::cout << "[ " << it->first << " ] -> " << it->second.id << std::endl;
		}
	} else {
		LOG(INFO) << "Nothing registered in event_list ==> OK";
	}
}
