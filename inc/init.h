#pragma once
/**
 * \author  Alex Krieg
 * \version 00.00.00
 * \date    04.10.2022
 *
 * \brief This class is used to register the QMetaType for signals but currently it also works without
 */

#include "settingGroup.h"
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
            }
        private:
    };

    //!< Static instance to call the constructor once at startup
    extern StaticInit m_instance;
}

