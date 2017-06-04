#include <pybind11/pybind11.h>
#include <lp3/log.hpp>
#include <lp3/core.hpp>
#include <windows.h>
#include <lp3/platform.hpp>

namespace core = lp3::core;
namespace py = pybind11;

namespace {

    void pylog(core::LogLevel level, const char * msg) {
        core::write_log("python", 0, level, msg);
    }

	core::SimulatedStopWatchRemainder simulator_stop_watch_run_updates(
		core::SimulatorStopWatch & watch,
		std::function<void(std::int64_t)> f) {
		return watch.run_updates(f);
	}

	void max_window(std::int64_t data) {
		HWND hwnd = reinterpret_cast<HWND>(data);
		::ShowWindow(hwnd, SW_MAXIMIZE);
		LP3_LOG_DEBUG("I DID IT!");
	}

	core::PlatformInitArgs create_init_args(std::int64_t data) {
		HWND hwnd = reinterpret_cast<HWND>(data);
		HINSTANCE hinstance = (HINSTANCE) ::GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
		return core::Platform::create_init_args(hinstance, hwnd);
	}
}

PYBIND11_PLUGIN(lp3py_core) {
    py::module m("lp3py_core", R"pbdoc(
        Lp3::Core
        ---------
        .. currentmodule:: lp3py_core
        .. autosummary::
           :toctree: _generate
           log
           subtract
    )pbdoc");
	
	py::class_<core::InputStream>(
		m, "InputStream")
		.def("eof", &core::InputStream::eof)
		.def("reset", &core::InputStream::reset)
		.def("skip", &core::InputStream::skip);

	py::class_<core::MediaManager>(
		m, "MediaManager")
		.def(py::init<>())
		.def("load", &core::MediaManager::load);

	py::class_<core::PlatformInitArgs>(
		m, "PlatformInitArgs");
	
	m.def("create_init_args", &create_init_args, R"pbdoc(
        Creates Init Args.
    )pbdoc");

    py::class_<core::SimulatedStopWatchRemainder>(
            m, "SimulatedStopWatchRemainder")
        .def(py::init<>())
        .def_readwrite("ms", &core::SimulatedStopWatchRemainder::ms)
        .def_readwrite("percent_of_frame", &core::SimulatedStopWatchRemainder::percent_of_frame);

	py::class_<core::SimulatorStopWatch>(
		m, "SimulatorStopWatch")
		.def(py::init<std::int64_t>())
		.def("run_updates", &simulator_stop_watch_run_updates);


    py::class_<core::Timer>(m, "Timer")
        .def(py::init<>())
        .def("cached_time", &core::Timer::cached_time)
        .def("get_time", &core::Timer::get_time);

    py::enum_<core::LogLevel>(m, "LogLevel")
        .value("DEBUG", core::LogLevel::Debug)
        .value("ERROR", core::LogLevel::Error)
        .value("INFO", core::LogLevel::Info)
        .value("WARNING", core::LogLevel::Warning)
        .export_values();

    m.def("log", &pylog, R"pbdoc(
        Logs to Squirrel Log.
    )pbdoc");

	m.def("max_window", &max_window, R"pbdoc(
        Maximizes window.
    )pbdoc");	

#ifdef VERSION_INFO
    m.attr("__version__") = py::str(VERSION_INFO);
#else
    m.attr("__version__") = py::str("dev");
#endif

    return m.ptr();
}
