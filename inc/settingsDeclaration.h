#pragma once

#define SETTINGS_WARNING_PRETTY qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << " : "
namespace Settings
{
    class Setting;
    class Settings;
}
