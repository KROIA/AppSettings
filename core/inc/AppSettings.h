// @file AppSettings.h
// @brief Main public header for the library.
//
// Include this single header to access the entire public API.
// Add your own public headers inside USER_SECTION 2 so that
// consumers only need `#include "AppSettings.h"`.
#pragma once

/// USER_SECTION_START 1

/// USER_SECTION_END

#include "AppSettings_info.h"

/// USER_SECTION_START 2
#include "Setting.h"
#include "ListSetting.h"
#include "MapSetting.h"
#include "SettingsGroup.h"
#include "ApplicationSettings.h"

#include "CustomVariants/Selection.h"

#include "ui/UI_AppSettingsEditor.h"
/// USER_SECTION_END