# Lp3 Sims

Code for handling simulations. Contains naive implementations for:

* stateless [coroutines](include/lp3/sims/coroutine.hpp)
* a [game clock](include/lp3/sims/Clock.hpp), which simplifies writing code that runs at the same speed regardless of the performance of the underlying hardware.
* [timers](include/lp3/sims/Clock.hpp) to measure FPS
* a super slow [event system](include/lp3/sims/EventSimulator.hpp) which handles dispatching to arbitrary functions using integral codes as well as causing code to fire at a specific time (this def. shouldn't be used in a tight loop and possibly shouldn't be used at all!)
