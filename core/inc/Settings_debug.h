#pragma once

// Profiling
#ifdef SETTINGS_PROFILING
#include <easy/profiler.h>
#include <easy/arbitrary_value.h> // EASY_VALUE, EASY_ARRAY are defined here

#define S_PROFILING_BLOCK_C(text, color) EASY_BLOCK(text, color)
#define S_PROFILING_NONSCOPED_BLOCK_C(text, color) EASY_NONSCOPED_BLOCK(text, color)
#define S_PROFILING_END_BLOCK EASY_END_BLOCK
#define S_PROFILING_FUNCTION_C(color) EASY_FUNCTION(color)
#define S_PROFILING_BLOCK(text, colorStage) S_PROFILING_BLOCK_C(text,profiler::colors::  colorStage)
#define S_PROFILING_NONSCOPED_BLOCK(text, colorStage) S_PROFILING_NONSCOPED_BLOCK_C(text,profiler::colors::  colorStage)
#define S_PROFILING_FUNCTION(colorStage) S_PROFILING_FUNCTION_C(profiler::colors:: colorStage)
#define S_PROFILING_THREAD(name) EASY_THREAD(name)

#define S_PROFILING_VALUE(name, value) EASY_VALUE(name, value)
#define S_PROFILING_TEXT(name, value) EASY_TEXT(name, value)

#else
#define S_PROFILING_BLOCK_C(text, color)
#define S_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define S_PROFILING_END_BLOCK
#define S_PROFILING_FUNCTION_C(color)
#define S_PROFILING_BLOCK(text, colorStage)
#define S_PROFILING_NONSCOPED_BLOCK(text, colorStage)
#define S_PROFILING_FUNCTION(colorStage)
#define S_PROFILING_THREAD(name)

#define S_PROFILING_VALUE(name, value)
#define S_PROFILING_TEXT(name, value)
#endif

// Special expantion tecniques are required to combine the color name
#define CONCAT_SYMBOLS_IMPL(x, y) x##y
#define CONCAT_SYMBOLS(x, y) CONCAT_SYMBOLS_IMPL(x, y)

// Different color stages
#define S_COLOR_STAGE_1 50
#define S_COLOR_STAGE_2 100
#define S_COLOR_STAGE_3 200
#define S_COLOR_STAGE_4 300
#define S_COLOR_STAGE_5 400
#define S_COLOR_STAGE_6 500
#define S_COLOR_STAGE_7 600
#define S_COLOR_STAGE_8 700
#define S_COLOR_STAGE_9 800
#define S_COLOR_STAGE_10 900
#define S_COLOR_STAGE_11 A100 
#define S_COLOR_STAGE_12 A200 
#define S_COLOR_STAGE_13 A400 
#define S_COLOR_STAGE_14 A700 

// General
#define S_GENERAL_PROFILING_COLORBASE Cyan
#define S_GENERAL_PROFILING_BLOCK_C(text, color) S_PROFILING_BLOCK_C(text, color)
#define S_GENERAL_PROFILING_NONSCOPED_BLOCK_C(text, color) S_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define S_GENERAL_PROFILING_END_BLOCK S_PROFILING_END_BLOCK;
#define S_GENERAL_PROFILING_FUNCTION_C(color) S_PROFILING_FUNCTION_C(color)
#define S_GENERAL_PROFILING_BLOCK(text, colorStage) S_PROFILING_BLOCK(text, CONCAT_SYMBOLS(S_GENERAL_PROFILING_COLORBASE, colorStage))
#define S_GENERAL_PROFILING_NONSCOPED_BLOCK(text, colorStage) S_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(S_GENERAL_PROFILING_COLORBASE, colorStage))
#define S_GENERAL_PROFILING_FUNCTION(colorStage) S_PROFILING_FUNCTION(CONCAT_SYMBOLS(S_GENERAL_PROFILING_COLORBASE, colorStage))
#define S_GENERAL_PROFILING_VALUE(name, value) S_PROFILING_VALUE(name, value)
#define S_GENERAL_PROFILING_TEXT(name, value) S_PROFILING_TEXT(name, value)


