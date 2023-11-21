#pragma once

#include "Settings_base.h"

#define SETTINGS_WARNING_PRETTY qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << " : "
namespace Settings
{
    class Setting;
    class JsonSetting;

    class SettingGroup;
    class JsonSettingGroup;
}
