#ifndef FILE_LP3_SIMS_DISPATCHER_HPP
#define FILE_LP3_SIMS_DISPATCHER_HPP

#include "config.hpp"
#include <functional>
#include <map>
#include <optional>
#include <typeindex>
#include <vector>


// amount of time they should sleep before

namespace lp3 { namespace sims {

using EventType = int;

using SubscriptionFuncId = int;

struct SubscriptionId {
	EventType event_type;
	SubscriptionFuncId func_id;
};

class EventSubscriber {
public:
    EventSubscriber();
    virtual ~EventSubscriber() {}
	virtual void operator()(void * args) = 0;

    bool cancelled;
    SubscriptionFuncId id;
};

template<typename T>
class EventSubscriberT : public EventSubscriber {
public:
    EventSubscriberT(std::function<void(const T & args)> func)
    :   EventSubscriber(),
        function(func)
    {}

    void operator()(void * args) override {
        const T * t_ptr = static_cast<const T *>(args);
        function(*t_ptr);
    }

    ~EventSubscriberT() override {
    }
private:
	std::function<void(T args)> function;
};

class EventSubscribers : public EventSubscriber {
public:
	EventSubscribers();
	~EventSubscribers() override;

    template<typename T>
    SubscriptionFuncId add_subscriber(std::function<void(T args)> func) {
        subscribers.push_back(new EventSubscriberT<T>(func));
        return subscribers.back()->id;
    }

    void cancel_subscriber(SubscriptionFuncId id);

	void operator()(void * args) override;

	void prune();

private:
	bool need_prune;
	std::vector<EventSubscriber *> subscribers;
};

class EventDispatcher {
public:
    template<typename T>
    void send(EventType id, T args) {
        void * v_ptr = static_cast<void *>(&args);
        return send_void_ptr(id, v_ptr, std::type_index(typeid(T)));
    }

	void send_void_ptr(const EventType id, void * ptr,
		               const std::type_index & type_index);

	EventSubscribers & find_or_create_subscribers(
		const EventType id,
		const std::type_index & type_index);

    template<typename T>
	SubscriptionId subscribe(const EventType id,
		                     std::function<void(T args)> func) {
        EventSubscribers & subscribers
            = find_or_create_subscribers(id, std::type_index(typeid(T)));
        SubscriptionFuncId fid = subscribers.add_subscriber(func);
		return { id, fid };
    }

	void prune();

    void unsubscribe(SubscriptionId);

private:
	std::map<EventType, EventSubscribers> subscriber_map;
	std::map<EventType, std::optional<std::type_index>> type_map;
};

}}

#endif
