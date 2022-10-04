#include "setting.h"

namespace Settings
{
Setting::Setting()
    : QObject()
{

}
Setting::Setting(const Setting &other)
    : QObject()
{
    m_parameter = other.m_parameter;
}
Setting::Setting(const QString &name, const QVariant value)
{
    m_parameter.first = name;
    m_parameter.second = value;
}
Setting::Setting(const std::pair<QString,QVariant> &setting)
{
    m_parameter = setting;
}
Setting::~Setting()
{

}

const Setting &Setting::operator=(const Setting &other)
{
    setName(other.m_parameter.first);
    setValue(other.m_parameter.second);
    return *this;
}
const Setting &Setting::operator=(const QVariant &value)
{
    setValue(value);
    return *this;
}
const QVariant &Setting::getValue() const
{
    return m_parameter.second;
}
const QString &Setting::getName() const
{
    return m_parameter.first;
}
const std::pair<QString,QVariant> &Setting::getPair() const
{
    return m_parameter;
}

QString Setting::toString() const
{
    return "{ "+m_parameter.first+" = "+m_parameter.second.toString()+" }";
}
QDebug operator<<(QDebug debug, const Setting &setting)
{
    debug.nospace() << setting.toString();
    return debug;
}

void Setting::setValue(const QVariant &value)
{
    if(m_parameter.second == value) return;
    m_parameter.second = value;
    emit valueChanged(m_parameter.second);
}
void Setting::setName(const QString &name)
{
    if(m_parameter.first == name) return;
    m_parameter.first = name;
    emit nameChanged(m_parameter.first);
}
}
