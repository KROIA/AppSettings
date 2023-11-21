#pragma once

#include <chrono>

#define SETTINGS_VERSION_MAJOR 0
#define SETTINGS_VERSION_MINOR 0
#define SETTINGS_VERSION_PATCH 0

#define SETTINGS_QT

#ifdef BUILD_STATIC
#define SETTINGS_STATIC
#endif

#ifndef SETTINGS_STATIC
	#if defined(SETTINGS_LIB)
		#define SETTINGS_EXPORT __declspec(dllexport)
	#else
		#define SETTINGS_EXPORT __declspec(dllimport)
	#endif
#else
	#define SETTINGS_EXPORT
#endif


// MSVC Compiler
#ifdef _MSC_VER 
	#define __PRETTY_FUNCTION__ __FUNCSIG__
	typedef std::chrono::steady_clock::time_point TimePoint;
#else
	typedef std::chrono::system_clock::time_point TimePoint;
#endif