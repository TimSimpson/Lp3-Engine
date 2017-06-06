#include <lp3/sims/qroutines.hpp>
#include <algorithm>
#include <lp3/assert.hpp>
#include <iostream>

namespace lp3 { namespace sims {

namespace {
    static QRoutineId next_id = 0;
}

QRoutine::QRoutine(QRoutineDelegate f, SleepTime _sleep_time)
:   func(f),
    sleep_time(_sleep_time),
    id(++ next_id)
{}

bool comp(const QRoutine & a, const QRoutine & b) {
	if (a.sleep_time < b.sleep_time) {
		return true;
	}
	if (a.sleep_time > b.sleep_time) {
		return false;
	}
	return a.id < b.id;
}

bool equiv(const QRoutine & a, const QRoutine & b) {
	return a.id == b.id;
}

bool comp(const QRoutine * a, const QRoutine * b) {
	LP3_ASSERT(nullptr != a);
	LP3_ASSERT(nullptr != b);
	return comp(*a, *b);
}

bool equiv(const QRoutine * a, const QRoutine * b) {
	LP3_ASSERT(nullptr != a);
	LP3_ASSERT(nullptr != b);
	return equiv(*a, *b);
}


QRoutineRunner::QRoutineRunner()
:   current_time_processed(0),
	routines()
{
}

void QRoutineRunner::cancel(const QRoutineId id) {
	auto itr = std::find_if(routines.begin(), routines.end(), [id](QRoutine & q) {
		return q.id == id;
	});
	if (itr != routines.end()) {
		routines.erase(itr);
	}
}

bool QRoutineRunner::process_events(const SleepTime time) {
	// As we process, new routines may spawn. However long their sleep time
	// is needs to be incremented by however far we're into processing this.

	// Let's routine A sleeps for 30, then sleeps for 10, then ends, and
	// routine B sleeps for 35. If we did not adjust the sleep time of routine
	// A it would finish completely before routine B had a chance to start,
	// as 10 > 35. However with the addition A's sleep time after it's first
	// iteration will be 40, not 10.

    LP3_ASSERT(list_is_ordered());

	current_time_processed = 0;
	auto itr = routines.begin();
	while (itr != routines.end() && itr->sleep_time <= time) {
		current_time_processed = itr->sleep_time;
		boost::optional<int> result = itr->func();
		if (result) {
            LP3_ASSERT(list_is_ordered());
			itr->sleep_time += result.get();
            // Move it back
			auto start = std::find_if(routines.begin(), routines.end(),
				[itr](const QRoutine & r) {
				return r.sleep_time > itr->sleep_time;
			});
			routines.splice(start, routines, itr);
            LP3_ASSERT(list_is_ordered());
		} else {
            LP3_ASSERT(list_is_ordered());
			itr = routines.erase(itr);
            LP3_ASSERT(list_is_ordered());
		}
        itr = routines.begin();
	}

	// Now iterate through and reduce the amount of sleep time from everything
	// by however much this loop was supposed to process.
	for (QRoutine & r : routines) {
		r.sleep_time -= time;
	}
	current_time_processed = 0;
	return proc_count() != 0;
}

bool QRoutineRunner::list_is_ordered() {
    SleepTime last_value  = 0;
    for (QRoutine & r : routines) {
        if (r.sleep_time < last_value) {
            return false;
        }
        last_value = r.sleep_time;
    }
    return true;
}


QRoutineId QRoutineRunner::run(QRoutineDelegate rd, SleepTime sleep_time) {
    LP3_ASSERT(list_is_ordered());
	sleep_time = sleep_time + current_time_processed;
	auto start = std::find_if(routines.begin(), routines.end(),
		[sleep_time](const QRoutine & r) {
			return r.sleep_time > sleep_time;
		});
	auto itr = routines.emplace(start, rd, sleep_time);
    LP3_ASSERT(list_is_ordered());
    return itr->id;
}

}   }
