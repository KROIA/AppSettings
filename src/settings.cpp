#include "settingGroup.h"

namespace Settings
{
using std::vector;
SettingGroup::SettingGroup()
{

}
SettingGroup::SettingGroup(const QString &name)
{
    setName(name);
}
SettingGroup::SettingGroup(const SettingGroup &other)
{
    m_name = other.m_name;
    m_SettingGroup.reserve(other.m_SettingGroup.size());
    for(size_t i=0; i<other.m_SettingGroup.size(); ++i)
    {
        Setting *newSetting = new Setting(*other.m_SettingGroup[i], this);
        connectSignals(newSetting);
        m_SettingGroup.push_back(newSetting);
    }
}
SettingGroup::~SettingGroup()
{
    for(size_t i=0; i<m_SettingGroup.size(); ++i)
    {
        disconnectSignals(m_SettingGroup[i]);
        delete m_SettingGroup[i];
    }
    m_SettingGroup.clear();
}
void SettingGroup::setName(const QString &name)
{
    m_name = name;
}
const QString &SettingGroup::getName() const
{
    return m_name;
}

const SettingGroup &SettingGroup::operator=(const SettingGroup &other)
{
    m_name = other.m_name;
    m_SettingGroup.reserve(other.m_SettingGroup.size());
    for(size_t i=0; i<other.m_SettingGroup.size(); ++i)
    {
        Setting *newSetting = new Setting(*other.m_SettingGroup[i], this);
        connectSignals(newSetting);
        m_SettingGroup.push_back(newSetting);
    }
    return *this;
}

Setting &SettingGroup::operator[](const QString &name)
{
    if(!exists(name))
    {
        static Setting invalidDummy;
        invalidDummy = Setting();
        SETTINGS_WARNING_PRETTY<< "name: "
                   << name << " is not in the SettingGroup list: " << m_name << "";
        return invalidDummy;
    }
    return *m_SettingGroup[getIndex(name)];
}
Setting &SettingGroup::operator[](size_t index)
{
    if(index < m_SettingGroup.size())
        return *m_SettingGroup[index];
    static Setting invalidDummy;
    invalidDummy = Setting();
    SETTINGS_WARNING_PRETTY<< "index: "
               << index << " is out of range, list "<<m_name<< " has only: " << m_SettingGroup.size()<< " SettingGroup";
    return invalidDummy;
}
bool SettingGroup::add(const Setting &setting)
{
    if(exists(setting.getName()))
    {
        SETTINGS_WARNING_PRETTY<< "SettingGroup list: " << m_name
                   << " already contains such a setting: " << m_SettingGroup[getIndex(setting.getName())]->toString();
        return false;
    }
    Setting *newSetting = new Setting(setting, this);
    connectSignals(newSetting);
    m_SettingGroup.push_back(newSetting);
    emit settingAdded(*newSetting);
    return true;
}
bool SettingGroup::add(const QString &name, const QVariant value)
{
    if(exists(name))
    {
        SETTINGS_WARNING_PRETTY<< "SettingGroup list: " << m_name
                   << " already contains such a setting: " << m_SettingGroup[getIndex(name)]->toString();
        return false;
    }
    Setting *newSetting = new Setting(name,value, this);
    connectSignals(newSetting);
    m_SettingGroup.push_back(newSetting);
    emit settingAdded(*newSetting);
    return true;
}
bool SettingGroup::add(const std::pair<QString,QVariant> &setting)
{
    if(exists(setting.first))
    {
        SETTINGS_WARNING_PRETTY<< "SettingGroup list: " << m_name
                   << " already contains such a setting: " << m_SettingGroup[getIndex(setting.first)]->toString();
        return false;
    }
    Setting *newSetting = new Setting(setting, this);
    connectSignals(newSetting);
    m_SettingGroup.push_back(newSetting);
    emit settingAdded(*newSetting);
    return true;
}
bool SettingGroup::remove(const Setting &setting)
{
    return remove(setting.getName());
}
bool SettingGroup::remove(const QString &name)
{
    if(exists(name))
    {
        size_t index = getIndex(name);
        Setting *setting = m_SettingGroup[index];
        m_SettingGroup.erase(m_SettingGroup.begin() + index);
        disconnectSignals(setting);
        delete setting;
        emit settingRemoved();
        return true;
    }
    return false;
}
bool SettingGroup::remove(size_t index)
{
    if(m_SettingGroup.size() >= index)
    {
        Setting *setting = m_SettingGroup[index];
        m_SettingGroup.erase(m_SettingGroup.begin() + index);
        disconnectSignals(setting);
        delete setting;
        emit settingRemoved();
        return true;
    }
    return false;
}
void SettingGroup::clear()
{
    for(size_t i=0; i<m_SettingGroup.size(); ++i)
    {
        disconnectSignals(m_SettingGroup[i]);
        delete m_SettingGroup[i];
    }
    m_SettingGroup.clear();
    emit SettingGroupCleared();
}


bool SettingGroup::exists(const QString &name) const
{
    for(size_t i=0; i<m_SettingGroup.size(); ++i)
    {
        if(m_SettingGroup[i])
            if(m_SettingGroup[i]->getName() == name)
                return true;
    }
    return false;
}
size_t SettingGroup::getIndex(const QString &name) const
{
    for(size_t i=0; i<m_SettingGroup.size(); ++i)
    {
        if(m_SettingGroup[i])
            if(m_SettingGroup[i]->getName() == name)
                return i;
    }
    return npos;
}
size_t SettingGroup::getSize() const
{
    return m_SettingGroup.size();
}
QString SettingGroup::toString(int tabCount) const
{
    QString tabs(tabCount*4,' ');
    const static QString singleTab = "    ";
    QString str = tabs + "SettingGroup: " + m_name + "\n" + tabs + "{";

    for(auto const &param : m_SettingGroup)
    {
        if(param)
            str += tabs + singleTab + param->toString() + "\n";
        else
            str += tabs + singleTab + "nullptr\n";
    }
    str+=tabs+"}";
    return str;
}
QDebug operator<<(QDebug debug, const SettingGroup &SettingGroup)
{
    debug.nospace() << SettingGroup.toString();
    return debug;
}
void SettingGroup::onSettingValueChanged(const QVariant&)
{
    Setting* setting = qobject_cast<Setting*>(QObject::sender());
    if(!setting)
    {
        SETTINGS_WARNING_PRETTY<< "receiving signal but sender is nullptr?!";
        return;
    }
    emit settingValueChanged(*setting);
}
void SettingGroup::onSettingNameChanged(const QString &)
{
    Setting* setting = qobject_cast<Setting*>(QObject::sender());
    if(!setting)
    {
        SETTINGS_WARNING_PRETTY<< "receiving signal but sender is nullptr?!";
        return;
    }
    emit settingNameChanged(*setting);
}
void SettingGroup::onSettingDestroyed(QObject *obj)
{
    Setting* setting = qobject_cast<Setting*>(obj);
    for(size_t i=0; i<m_SettingGroup.size(); ++i)
    {
        if(m_SettingGroup[i] == setting)
        {
            m_SettingGroup.erase(m_SettingGroup.begin() + i);
            emit settingRemoved();
            return;
        }
    }
}
void SettingGroup::connectSignals(Setting *setting)
{
    if(!setting) return;
    connect(setting,  &Setting::nameChanged,  this,  &SettingGroup::onSettingNameChanged);
    connect(setting,  &Setting::valueChanged, this,  &SettingGroup::onSettingValueChanged);
    connect(setting,  &Setting::destroyed,    this,  &SettingGroup::onSettingDestroyed);
}
void SettingGroup::disconnectSignals(Setting *setting)
{
    if(!setting) return;
    disconnect(setting,  &Setting::nameChanged,  this,  &SettingGroup::onSettingNameChanged);
    disconnect(setting,  &Setting::valueChanged, this,  &SettingGroup::onSettingValueChanged);
    disconnect(setting,  &Setting::destroyed,    this,  &SettingGroup::onSettingDestroyed);
}
}
