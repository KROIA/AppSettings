#pragma once
/**
 * \author  Alex Krieg
 * \version 00.00.01
 * \date    27.10.2022
 *
 * \brief This class is used to register the QMetaType for signals but currently it also works without
 */

#include "JsonSetting.h"
#include "JsonSettingGroup.h"
#include <QDebug>

namespace Settings
{
    //!< Static class which is only used once, to call its constructor
    class StaticInit
    {
        public:
            StaticInit(){
               qRegisterMetaType<Setting>("Setting");
               qRegisterMetaType<SettingGroup>("SettingGroup");
               qRegisterMetaType<JsonSetting>("JsonSetting");
               qRegisterMetaType<JsonSettingGroup>("JsonSettingGroup");
            }
        private:
    };

    //!< Static instance to call the constructor once at startup
    extern StaticInit m_instance;
}

