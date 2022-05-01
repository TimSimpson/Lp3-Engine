#include <lp3/sims/dispatcher.hpp>
#include <lp3/core.hpp>

namespace lp3 { namespace sims {

namespace {
	static SubscriptionFuncId next_id = 0;
}

EventSubscriber::EventSubscriber()
:	cancelled(false), id(++ next_id) {
}

EventSubscribers::EventSubscribers()
:	need_prune(false) {
}

EventSubscribers::~EventSubscribers() {
	for (EventSubscriber * ptr : subscribers) {
		delete ptr;
	}
}

void EventSubscribers::operator ()(void * args) {
	for (EventSubscriber * es : subscribers) {
		(*es)(args);
	}
}

void EventSubscribers::cancel_subscriber(SubscriptionFuncId sf_id) {
	for (EventSubscriber * s : this->subscribers) {
		LP3_ASSERT(nullptr != s);
        if (s->id == sf_id) {
            s->cancelled = true;
			need_prune = true;
        }
    }
}

void EventSubscribers::prune() {
	if (need_prune) {
		subscribers.erase(
			std::remove_if(
				subscribers.begin(),
				subscribers.end(),
				[](const EventSubscriber * s) {
					if (s->cancelled) {
						delete s;
						return true;
					}
					return false;
				}),
			subscribers.end()
		);
	}
}

void EventDispatcher::send_void_ptr(
	EventType id,
	void * ptr,
	const std::type_index & type_index)
{
	auto itr = subscriber_map.find(id);
	if (itr == subscriber_map.end()) {
		return;
	}
	LP3_ASSERT(type_index == type_map[id].value());
	EventSubscribers & subscribers = itr->second;
	subscribers(ptr);
}

EventSubscribers & EventDispatcher::find_or_create_subscribers(
	const EventType id,
	const std::type_index & type_index)
{
	auto itr = subscriber_map.find(id);
	if (itr == subscriber_map.end()) {
		subscriber_map[id] = EventSubscribers{};
		type_map[id] = type_index;
	}
	else {
		LP3_ASSERT(type_index == type_map[id].value());
	}
	return subscriber_map[id];
}

void EventDispatcher::prune() {
	for (auto & element : subscriber_map) {
		element.second.prune();
	}
}

void EventDispatcher::unsubscribe(SubscriptionId id) {
	auto itr = subscriber_map.find(id.event_type);
	if (itr == subscriber_map.end()) {
		LP3_ASSERT(false);
		return;
	} else {
		itr->second.cancel_subscriber(id.func_id);
	}
}


}	}
