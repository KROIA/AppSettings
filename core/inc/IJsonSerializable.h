#pragma once

#include "AppSettings_base.h"
#include <QJsonObject>

namespace AppSettings
{
    class IJsonSerializable
    {
        public:
            virtual void save(QJsonObject &settings) const = 0;
            virtual bool read(const QJsonObject &reader) = 0;
    };
}
