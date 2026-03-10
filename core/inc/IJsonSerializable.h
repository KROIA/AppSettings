#pragma once


class QJsonObject;
namespace AppSettings
{
    class IJsonSerializable
    {
        public:
            virtual void save(QJsonObject &settings) const = 0;
            virtual bool load(const QJsonObject &reader) = 0;
    };
}
