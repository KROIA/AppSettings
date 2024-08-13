#include "setting.h"


namespace AppSettings
{
Setting::Setting()
    : ISetting()
{
    m_parameter.first = "Setting";
}
Setting::Setting(const Setting &other)
    : ISetting()
{
    m_parameter = other.m_parameter;
}
Setting::Setting(const QString &name, const QVariant &value)
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

bool Setting::operator==(const Setting& other)
{
    if(m_parameter.second == other.m_parameter.second)
        return true;
    return false;
}
bool Setting::operator==(const QVariant& otherValue)
{
    if(m_parameter.second == otherValue)
        return true;
    return false;
}
bool Setting::operator!=(const Setting& other)
{
    if(m_parameter.second != other.m_parameter.second)
        return true;
    return false;
}
bool Setting::operator!=(const QVariant& otherValue)
{
    if(m_parameter.second != otherValue)
        return true;
    return false;
}

Setting &Setting::operator=(const Setting &other)
{
    setName(other.m_parameter.first);
    setValue(other.m_parameter.second);
    return *this;
}
Setting &Setting::operator=(const QVariant &value)
{
    setValue(value);
    return *this;
}
const QVariant &Setting::getValue() const
{
    return m_parameter.second;
}
QString Setting::getName() const
{
    return m_parameter.first;
}
const std::pair<QString,QVariant> &Setting::getPair() const
{
    return m_parameter;
}

QString Setting::toString() const
{
    return m_parameter.first+" = "+m_parameter.second.toString();
}
QDebug operator<<(QDebug debug, const Setting &setting)
{
    debug.nospace() << setting.toString();
    return debug;
}
std::ostream& operator<<(std::ostream& stream, const Setting& setting)
{
    stream << setting.toString().toStdString();
	return stream;
}

void Setting::save(QJsonObject& settings) const
{
    settings[m_parameter.first] = QJsonValue::fromVariant(m_parameter.second);
}
bool Setting::load(const QJsonObject& reader)
{
    if (reader.find(m_parameter.first) == reader.end())
    {
        AS_CONSOLE_FUNCTION("Unable to read setting: " << m_parameter.first.toStdString()
		<< ". Setting not found");
		return false;
	}
	else
	{
        setValue(reader[m_parameter.first].toVariant());
		return true;
	}
}

void Setting::setValue(const QVariant &value)
{
    if(m_parameter.second == value) return;
    m_parameter.second = value;
    emit valueChanged();
}
void Setting::setName(const QString &name)
{
    if(m_parameter.first == name) return;
    if(name == "")
    {
        AS_CONSOLE_FUNCTION("Unable to change name from" << m_parameter.first.toStdString()
        << " to " << name.toStdString() << "\nnew name is empty");
        return;
    }
    m_parameter.first = name;
    emit nameChanged();
}
}
