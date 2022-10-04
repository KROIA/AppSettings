#include "settings.h"

namespace Settings
{

Settings::Settings()
{

}
Settings::Settings(const QString &name)
{
    setName(name);
}
Settings::Settings(const Settings &other)
{
    m_settings.reserve(other.m_settings.size());
    for(size_t i=0; i<other.m_settings.size(); ++i)
    {
        Setting *newSetting = new Setting(*other.m_settings[i]);
        connectSignals(newSetting);
        m_settings.push_back(newSetting);
    }
}
Settings::~Settings()
{
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        disconnectSignals(m_settings[i]);
        delete m_settings[i];
    }
    m_settings.clear();
}
void Settings::setName(const QString &name)
{
    m_name = name;
}
const QString &Settings::getName() const
{
    return m_name;
}

const Settings &Settings::operator=(const Settings &other)
{
    m_settings.reserve(other.m_settings.size());
    for(size_t i=0; i<other.m_settings.size(); ++i)
    {
        Setting *newSetting = new Setting(*other.m_settings[i]);
        connectSignals(newSetting);
        m_settings.push_back(newSetting);
    }
    return *this;
}

Setting &Settings::operator[](const QString &name)
{
    if(!exists(name))
    {
        static Setting invalidDummy;
        invalidDummy = Setting();
        qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << "name: "
                   << name << " is not in the settings list: " << m_name << "";
        return invalidDummy;
    }
    return *m_settings[getIndex(name)];
}
Setting &Settings::operator[](size_t index)
{
    if(index < m_settings.size())
        return *m_settings[index];
    static Setting invalidDummy;
    invalidDummy = Setting();
    qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << "index: "
               << index << " is out of range, list "<<m_name<< " has only: " << m_settings.size()<< " settings";
    return invalidDummy;
}
bool Settings::add(const Setting &setting)
{
    if(exists(setting.getName()))
    {
        qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << "settings list: " << m_name
                   << " already contains such a setting: " << m_settings[getIndex(setting.getName())]->toString();
        return false;
    }
    Setting *newSetting = new Setting(setting);
    connectSignals(newSetting);
    m_settings.push_back(newSetting);
    emit settingAdded();
    return true;
}
bool Settings::add(const QString &name, const QVariant value)
{
    if(exists(name))
    {
        qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << "settings list: " << m_name
                   << " already contains such a setting: " << m_settings[getIndex(name)]->toString();
        return false;
    }
    Setting *newSetting = new Setting(name,value);
    connectSignals(newSetting);
    m_settings.push_back(newSetting);
    emit settingAdded();
    return true;
}
bool Settings::add(const std::pair<QString,QVariant> &setting)
{
    if(exists(setting.first))
    {
        qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << "settings list: " << m_name
                   << " already contains such a setting: " << m_settings[getIndex(setting.first)]->toString();
        return false;
    }
    Setting *newSetting = new Setting(setting);
    connectSignals(newSetting);
    m_settings.push_back(newSetting);
    emit settingAdded();
    return true;
}
bool Settings::remove(const Setting &setting)
{
    return remove(setting.getName());
}
bool Settings::remove(const QString &name)
{
    if(exists(name))
    {
        size_t index = getIndex(name);
        Setting *setting = m_settings[index];
        m_settings.erase(m_settings.begin() + index);
        disconnectSignals(setting);
        delete setting;
        emit settingRemoved();
        return true;
    }
    return false;
}
bool Settings::remove(size_t index)
{
    if(m_settings.size() >= index)
    {
        Setting *setting = m_settings[index];
        m_settings.erase(m_settings.begin() + index);
        disconnectSignals(setting);
        delete setting;
        emit settingRemoved();
        return true;
    }
    return false;
}
void Settings::clear()
{
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        disconnectSignals(m_settings[i]);
        delete m_settings[i];
    }
    m_settings.clear();
    emit settingsCleared();
}


bool Settings::exists(const QString &name) const
{
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        if(m_settings[i])
            if(m_settings[i]->getName() == name)
                return true;
    }
    return false;
}
size_t Settings::getIndex(const QString &name) const
{
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        if(m_settings[i])
            if(m_settings[i]->getName() == name)
                return i;
    }
    return 0;
}
size_t Settings::getSize() const
{
    return m_settings.size();
}
QString Settings::toString(int tabCount) const
{
    QString tabs(tabCount+2,' ');
    QString str = tabs + "settings: " + m_name + "\n" + tabs + "{";

    for(auto const &param : m_settings)
    {
        if(param)
            str += tabs + param->toString() + "\n";
        else
            str += tabs + "nullptr\n";
    }
    str+=tabs+"}";
    return str;
}
QDebug operator<<(QDebug debug, const Settings &settings)
{
    debug.nospace() << settings.toString();
    return debug;
}

void Settings::onSettingValueChanged(const QVariant &value)
{
    Setting* setting = qobject_cast<Setting*>(QObject::sender());
    if(!setting)
    {
        qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << "receiving signal but sender is nullptr?!";
        return;
    }
    emit settingValueChanged(*setting);
}
void Settings::onSettingNameChanged(const QString &parameterName)
{
    Setting* setting = qobject_cast<Setting*>(QObject::sender());
    if(!setting)
    {
        qWarning() << __FILE__ << "->" << __PRETTY_FUNCTION__ << "receiving signal but sender is nullptr?!";
        return;
    }
    emit settingNameChanged(*setting);
}
void Settings::onSettingDestroyed(QObject *obj)
{
    Setting* setting = qobject_cast<Setting*>(obj);
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        if(m_settings[i] == setting)
        {
            m_settings.erase(m_settings.begin() + i);
            emit settingRemoved();
            return;
        }
    }
}
void Settings::connectSignals(Setting *setting)
{
    if(!setting) return;
    connect(setting,  &Setting::nameChanged,  this,  &Settings::onSettingNameChanged);
    connect(setting,  &Setting::valueChanged, this,  &Settings::onSettingValueChanged);
    connect(setting,  &Setting::destroyed,    this,  &Settings::onSettingDestroyed);
}
void Settings::disconnectSignals(Setting *setting)
{
    if(!setting) return;
    disconnect(setting,  &Setting::nameChanged,  this,  &Settings::onSettingNameChanged);
    disconnect(setting,  &Setting::valueChanged, this,  &Settings::onSettingValueChanged);
    disconnect(setting,  &Setting::destroyed,    this,  &Settings::onSettingDestroyed);
}
}
