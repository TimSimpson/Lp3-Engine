# Change Log

### 1.0.2 2020-09-26

Created a change log.

Also changed everything around again to make Lp3-Engine produce a valid Conan package locally.

I'm using the new cpp_info components feature so the various pieces of this thing can be used in CMake with names like "lp3::gfx", "lp3::input", etc. Seems to work with the test_package but time will tell if I made some mistakes. My guess is a lot is still going to change in the next little while.

I'd like to make sure that the CMake stuff still works from a parent project and from traditional CMake based packaging tools like cget, though my guess is it's currently not working. It's too hard to maintain tests in both cget and conan so for now I think I'm cutting ties to the old more standard approaches with CMake and just rolling forward with Conan.
