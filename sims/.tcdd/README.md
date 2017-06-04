# Lp3 Sims

[![Build status](https://ci.appveyor.com/api/projects/status/k1v4yvke6if7goft?svg=true)](https://ci.appveyor.com/project/TimSimpson/lp3-sims)
[![Build Status](https://travis-ci.org/TimSimpson/Lp3-Sims.svg?branch=master)](https://travis-ci.org/TimSimpson/Lp3-Sims)

Code for hndling simulations. Contains naive implementations for:

* stateless [coroutines](include/lp3/sims/coroutine.hpp)
* a [game clock](include/lp3/sims/Clock.hpp), which simplifies writing code that runs at the same speed regardless of the performance of the underlying hardware.
* [timers](include/lp3/sims/Clock.hpp) to measure FPS
* a super slow [event system](include/lp3/sims/EventSimulator.hpp) which handles dispatching to arbitrary functions using integral codes as well as causing code to fire at a specific time (this def. shouldn't be used in a tight loop and possibly shouldn't be used at all!)

## Building

{{ readme_build }}

### Dependencies / Requirements

{{ readme_deps }}
