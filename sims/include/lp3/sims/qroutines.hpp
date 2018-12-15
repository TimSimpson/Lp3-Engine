#ifndef FILE_LP3_SIMS_QROUTINES_HPP
#define FILE_LP3_SIMS_QROUTINES_HPP

#include "config.hpp"
#include <functional>
#include <list>
#include <vector>


// "Queue Routines"
// These are coroutines featuring delayed execution. They can be run in some
// given amount of time in the future, and they return the time they should
// sleep before running agia.nEach always return the
// amount of time they should sleep before

namespace lp3 { namespace sims {

using QRoutineId = int;

using SleepTime = int;

using QRoutineDelegate = std::function<std::optional<SleepTime>()>;

class QRoutine {
public:
    QRoutine(QRoutineDelegate f, SleepTime _sleep_time);
	friend class QRoutineRunner;
	friend bool comp(const QRoutine & a, const QRoutine & b);
	friend bool equiv(const QRoutine & a, const QRoutine & b);
private:
    // Returns time to sleep until next call, or none if it should die.
    QRoutineDelegate func;

    SleepTime sleep_time;

    QRoutineId id;
};

bool comp(const QRoutine & a, const QRoutine & b);

bool equiv(const QRoutine & a, const QRoutine & b);

bool comp(const QRoutine * a, const QRoutine * b);

bool equiv(const QRoutine * a, const QRoutine * b);

class QRoutineRunner {
public:
    QRoutineRunner();

    void cancel(const QRoutineId id);

    bool process_events(SleepTime time);

    QRoutineId run(QRoutineDelegate rd, SleepTime sleep_time);

	inline std::size_t proc_count() const { return routines.size(); }

private:
	SleepTime current_time_processed;

    //std::vector<QRoutine *> routines;
	std::list<QRoutine> routines;

    bool list_is_ordered();
};

}}

#endif
