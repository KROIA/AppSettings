#pragma once

#include "Settings_base.h"
#include <QJsonObject>

namespace Settings
{
    class IJsonSerializable
    {
        public:
            virtual void save(QJsonObject &settings) const = 0;
            virtual bool read(const QJsonObject &reader) = 0;
    };
}
