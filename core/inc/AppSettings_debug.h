#pragma once

/// USER_SECTION_START 1

/// USER_SECTION_END

// Debugging
#ifdef NDEBUG
#define AS_CONSOLE(msg)
#define AS_CONSOLE_FUNCTION(msg)
#else
#include <iostream>

#define AS_DEBUG
#define AS_CONSOLE_STREAM std::cout

#define AS_CONSOLE(msg) AS_CONSOLE_STREAM << msg;
#define AS_CONSOLE_FUNCTION(msg) AS_CONSOLE_STREAM << __PRETTY_FUNCTION__ << " " << msg;
#endif

/// USER_SECTION_START 2

/// USER_SECTION_END

#ifdef AS_PROFILING
#include "easy/profiler.h"
#include <easy/arbitrary_value.h> // EASY_VALUE, EASY_ARRAY are defined here

#define AS_PROFILING_BLOCK_C(text, color) EASY_BLOCK(text, color)
#define AS_PROFILING_NONSCOPED_BLOCK_C(text, color) EASY_NONSCOPED_BLOCK(text, color)
#define AS_PROFILING_END_BLOCK EASY_END_BLOCK
#define AS_PROFILING_FUNCTION_C(color) EASY_FUNCTION(color)
#define AS_PROFILING_BLOCK(text, colorStage) AS_PROFILING_BLOCK_C(text,profiler::colors::  colorStage)
#define AS_PROFILING_NONSCOPED_BLOCK(text, colorStage) AS_PROFILING_NONSCOPED_BLOCK_C(text,profiler::colors::  colorStage)
#define AS_PROFILING_FUNCTION(colorStage) AS_PROFILING_FUNCTION_C(profiler::colors:: colorStage)
#define AS_PROFILING_THREAD(name) EASY_THREAD(name)

#define AS_PROFILING_VALUE(name, value) EASY_VALUE(name, value)
#define AS_PROFILING_TEXT(name, value) EASY_TEXT(name, value)

#else
#define AS_PROFILING_BLOCK_C(text, color)
#define AS_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define AS_PROFILING_END_BLOCK
#define AS_PROFILING_FUNCTION_C(color)
#define AS_PROFILING_BLOCK(text, colorStage)
#define AS_PROFILING_NONSCOPED_BLOCK(text, colorStage)
#define AS_PROFILING_FUNCTION(colorStage)
#define AS_PROFILING_THREAD(name)

#define AS_PROFILING_VALUE(name, value)
#define AS_PROFILING_TEXT(name, value)
#endif

// Special expantion tecniques are required to combine the color name
#define CONCAT_SYMBOLS_IMPL(x, y) x##y
#define CONCAT_SYMBOLS(x, y) CONCAT_SYMBOLS_IMPL(x, y)



// Different color stages
#define AS_COLOR_STAGE_1 50
#define AS_COLOR_STAGE_2 100
#define AS_COLOR_STAGE_3 200
#define AS_COLOR_STAGE_4 300
#define AS_COLOR_STAGE_5 400
#define AS_COLOR_STAGE_6 500
#define AS_COLOR_STAGE_7 600
#define AS_COLOR_STAGE_8 700
#define AS_COLOR_STAGE_9 800
#define AS_COLOR_STAGE_10 900
#define AS_COLOR_STAGE_11 A100 
#define AS_COLOR_STAGE_12 A200 
#define AS_COLOR_STAGE_13 A400 
#define AS_COLOR_STAGE_14 A700 


// General
#define AS_GENERAL_PROFILING_COLORBASE Cyan
#define AS_GENERAL_PROFILING_BLOCK_C(text, color) AS_PROFILING_BLOCK_C(text, color)
#define AS_GENERAL_PROFILING_NONSCOPED_BLOCK_C(text, color) AS_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define AS_GENERAL_PROFILING_END_BLOCK AS_PROFILING_END_BLOCK;
#define AS_GENERAL_PROFILING_FUNCTION_C(color) AS_PROFILING_FUNCTION_C(color)
#define AS_GENERAL_PROFILING_BLOCK(text, colorStage) AS_PROFILING_BLOCK(text, CONCAT_SYMBOLS(AS_GENERAL_PROFILING_COLORBASE, colorStage))
#define AS_GENERAL_PROFILING_NONSCOPED_BLOCK(text, colorStage) AS_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(AS_GENERAL_PROFILING_COLORBASE, colorStage))
#define AS_GENERAL_PROFILING_FUNCTION(colorStage) AS_PROFILING_FUNCTION(CONCAT_SYMBOLS(AS_GENERAL_PROFILING_COLORBASE, colorStage))
#define AS_GENERAL_PROFILING_VALUE(name, value) AS_PROFILING_VALUE(name, value)
#define AS_GENERAL_PROFILING_TEXT(name, value) AS_PROFILING_TEXT(name, value)


/// USER_SECTION_START 3

/// USER_SECTION_END