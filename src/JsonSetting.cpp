#include "JsonSetting.h"
#include "JsonSettingGroup.h"
namespace Settings
{

JsonSetting::JsonSetting()
    :   Setting()
    ,   IJsonSerializable()
{

}
JsonSetting::JsonSetting(const Setting &other)
    :   Setting(other)
    ,   IJsonSerializable()
{

}
JsonSetting::JsonSetting(const QString &name, const QVariant value)
    :   Setting(name, value)
    ,   IJsonSerializable()
{

}
JsonSetting::JsonSetting(const std::pair<QString,QVariant> &setting)
    :   Setting(setting)
    ,   IJsonSerializable()
{

}


QJsonObject JsonSetting::save() const
{
    return QJsonObject
    {
        // Add the properties of this object here
        // Do not take the same keyvalues two times,
        // also not the keys of the base class
        {"key" , m_parameter.first},
        {"val",  m_parameter.second.toString()},
    };
}
bool JsonSetting::read(const QJsonObject &reader)
{
    // Read the values for this class
    m_parameter.first = reader["key"].toString();
    QString valueStr = reader["val"].toString();
    bool okFloat = false;
    bool okInt = false;

    float floatVal = valueStr.toFloat(&okFloat);
    if(okFloat)
    {
        m_parameter.second = floatVal;
        return true;
    }

    int intVal = valueStr.toFloat(&okInt);
    if(okInt)
    {
        m_parameter.second = intVal;
        return true;
    }
    m_parameter.second = valueStr;
    return true;
}
}
