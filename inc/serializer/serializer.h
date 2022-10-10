#pragma once
#include "settingGroup.h"

namespace Settings
{
    class Serializer
    {
        public:

        virtual bool save(const Settings &settings) = 0;
        virtual bool load(Settings &settings) = 0;
    };
}
