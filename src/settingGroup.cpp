#include "SettingGroup.h"
#include "Setting.h"

namespace Settings
{
using std::vector;
SettingGroup::SettingGroup()
{
    m_isEmpty = true;
    m_parent = nullptr;
}
SettingGroup::SettingGroup(const QString &name)
{
    m_parent = nullptr;
    m_isEmpty = true;
    setName(name);
}
/*SettingGroup::SettingGroup(const SettingGroup &other, SettingGroup *parent)
{
    m_parent = parent;
    m_name = other.m_name;
    m_settings.reserve(other.m_settings.size());
    m_settingGroups.reserve(other.m_settingGroups.size());
    for(size_t i=0; i<other.m_settings.size(); ++i)
    {
        Setting *newSetting = new Setting(*other.m_settings[i], this);
        connectSignals(newSetting);
        m_settings.push_back(newSetting);
    }
    for(size_t i=0; i<other.m_settingGroups.size(); ++i)
    {
        SettingGroup *newSettings = new SettingGroup(*other.m_settingGroups[i], this);
        connectSignals(newSettings);
        m_settingGroups.push_back(newSettings);
    }
}*/
SettingGroup::~SettingGroup()
{
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        disconnectSignals(m_settings[i]);
        delete m_settings[i];
    }
    for(size_t i=0; i<m_settingGroups.size(); ++i)
    {
        disconnectSignals(m_settingGroups[i]);
        delete m_settingGroups[i];
    }
    m_settings.clear();
    m_settingGroups.clear();
}
SettingGroup *SettingGroup::getParent() const
{
    return m_parent;
}
void SettingGroup::setName(const QString &name)
{
    if(m_name == name) return;

    if(m_parent)
        if(m_parent->childGroupExists(name))
        {
            SETTINGS_WARNING_PRETTY << "Unable to change name from" << m_name
            << " to " << name << "\nsame name already exists in " << m_parent->getName();
            return;
        }
    if(name == "")
    {
        SETTINGS_WARNING_PRETTY << "Unable to change name from" << m_name
        << " to " << name << "\nnew name is empty";
        return;
    }
    m_name = name;
    emit nameChanged(m_name);
}
const QString &SettingGroup::getName() const
{
    return m_name;
}
bool SettingGroup::isEmpty() const
{
    return !(m_settingGroups.size() || m_settings.size());
}

/*const SettingGroup &SettingGroup::operator=(const SettingGroup &other)
{
    m_name = other.m_name;
    m_settings.reserve(other.m_settings.size());
    for(size_t i=0; i<other.m_settings.size(); ++i)
    {
        Setting *newSetting = new Setting(*other.m_settings[i], this);
        connectSignals(newSetting);
        m_settings.push_back(newSetting);
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
    return *m_settings[getIndex(name)];
}
Setting &SettingGroup::operator[](size_t index)
{
    if(index < m_settings.size())
        return *m_settings[index];
    static Setting invalidDummy;
    invalidDummy = Setting();
    SETTINGS_WARNING_PRETTY<< "index: "
               << index << " is out of range, list "<<m_name<< " has only: " << m_settings.size()<< " SettingGroup";
    return invalidDummy;
}*/
bool SettingGroup::add(Setting *setting)
{
    if(!setting)
        return false;
    if(settingExists(setting->getName()))
    {
        SETTINGS_WARNING_PRETTY<< "SettingGroup list: " << m_name
                   << " already contains such a setting: " << m_settings[getSettingIndex(setting->getName())]->toString();
        return false;
    }
    if(setting->getParent() != nullptr)
        setting->getParent()->removeSetting(setting);
    setting->Setting::setParent(this);
    connectSignals(setting);
    m_settings.push_back(setting);
    m_isEmpty = false;
    emit settingAdded(setting);
    return true;
}
Setting* SettingGroup::add(const QString &name, const QVariant &value)
{
    if(settingExists(name))
    {
        SETTINGS_WARNING_PRETTY<< "SettingGroup list: " << m_name
                   << " already contains such a setting: " << m_settings[getSettingIndex(name)]->toString();
        return nullptr;
    }
    Setting *newSetting = createSettingInstance();
    newSetting->setName(name);
    newSetting->setValue(value);
    newSetting->setParent(this);
    connectSignals(newSetting);
    m_settings.push_back(newSetting);
    m_isEmpty = false;
    emit settingAdded(newSetting);
    return newSetting;
}
bool SettingGroup::add(SettingGroup *group)
{
    if(!group)
        return false;
    if(group->getName() == "")
    {
        SETTINGS_WARNING_PRETTY<< "Group can't be added to " << m_name
                   << ". The group you try to add has no name";
        return false;
    }
    if(childGroupExists(group->getName()))
    {
        SETTINGS_WARNING_PRETTY<< "SettingGroup list: " << m_name
                   << " already contains such a settings group: " << m_settingGroups[getChildGroupIndex(group->getName())]->toString();
        return false;
    }
    if(group->m_parent)
        group->m_parent->removeGroup(group);
    group->m_parent = this;
    connectSignals(group);
    m_settingGroups.push_back(group);
    m_isEmpty = false;
    emit childGroupAdded(group);
    return true;
}
/*bool SettingGroup::add(const std::pair<QString,QVariant> &setting)
{
    if(exists(setting.first))
    {
        SETTINGS_WARNING_PRETTY<< "SettingGroup list: " << m_name
                   << " already contains such a setting: " << m_settings[getIndex(setting.first)]->toString();
        return false;
    }
    Setting *newSetting = new Setting(setting, this);
    connectSignals(newSetting);
    m_settings.push_back(newSetting);
    emit settingAdded(*newSetting);
    return true;
}*/
bool SettingGroup::removeSetting(Setting *setting)
{
    if(!setting)
        return false;
    return removeSetting(setting->getName());
}
Setting *SettingGroup::removeSetting(const QString &name)
{
    size_t index = getSettingIndex(name);
    return removeSetting(index);
}
Setting *SettingGroup::removeSetting(size_t index)
{
    if(m_settings.size() >= index)
    {
        Setting *setting = m_settings[index];
        m_settings.erase(m_settings.begin() + index);
        disconnectSignals(setting);
        setting->Setting::setParent(nullptr);
        if(isEmpty())
            m_isEmpty = true;
        emit settingRemoved(setting);
        return setting;
    }
    return nullptr;
}

SettingGroup *SettingGroup::removeGroup(SettingGroup *setting)
{
    if(!setting)
        return nullptr;
    return removeGroup(setting->getName());
}
SettingGroup *SettingGroup::removeGroup(const QString &name)
{
    size_t index = getChildGroupIndex(name);
    return removeGroup(index);
}
SettingGroup *SettingGroup::removeGroup(size_t index)
{
    if(m_settings.size() >= index)
    {
        SettingGroup *group = m_settingGroups[index];
        m_settingGroups.erase(m_settingGroups.begin() + index);
        disconnectSignals(group);
        group->m_parent = nullptr;
        if(isEmpty())
            m_isEmpty = true;
        emit childGroupRemoved(group);
        return group;
    }
    return nullptr;
}


void SettingGroup::clear()
{
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        disconnectSignals(m_settings[i]);
        m_settings[i]->Setting::setParent(nullptr);
    }
    for(size_t i=0; i<m_settingGroups.size(); ++i)
    {
        disconnectSignals(m_settingGroups[i]);
        m_settingGroups[i]->m_parent = nullptr;
    }
    m_settings.clear();
    m_settingGroups.clear();
    m_isEmpty = true;
    emit cleared();
}


bool SettingGroup::settingExists(const QString &name) const
{
    return getSettingIndex(name) != npos;
}
bool SettingGroup::childGroupExists(const QString &name) const
{
    return getChildGroupIndex(name) != npos;
}

size_t SettingGroup::getSettingIndex(const QString &name) const
{
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        if(m_settings[i])
            if(m_settings[i]->getName() == name)
                return i;
    }
    return npos;
}
size_t SettingGroup::getChildGroupIndex(const QString &name) const
{
    for(size_t i=0; i<m_settingGroups.size(); ++i)
    {
        if(m_settingGroups[i])
            if(m_settingGroups[i]->getName() == name)
                return i;
    }
    return npos;
}

Setting *SettingGroup::getSetting(size_t index) const
{
    if(index >= m_settings.size())
    {
        SETTINGS_WARNING_PRETTY<< "index out of range. index = "<<index << " max is = "<<m_settings.size();
        return nullptr;
    }
    return m_settings[index];
}
Setting *SettingGroup::getSetting(const QString &name) const
{
    size_t index = getSettingIndex(name);
    if(index == npos)
        return nullptr;
    return m_settings[index];
}
SettingGroup *SettingGroup::getChildGroup(const QString &name) const
{
    size_t index = getChildGroupIndex(name);
    if(index == npos)
        return nullptr;
    return m_settingGroups[index];
}
SettingGroup *SettingGroup::getChildGroup(size_t index) const
{
    if(index >= m_settingGroups.size())
    {
        SETTINGS_WARNING_PRETTY<< "index out of range. index = "<<index << " max is = "<<m_settingGroups.size();
        return nullptr;
    }
    return m_settingGroups[index];
}

size_t SettingGroup::getSettingsCount() const
{
    return m_settings.size();
}
size_t SettingGroup::getChildGroupCount() const
{
    return m_settingGroups.size();
}
QString SettingGroup::toString(int tabCount) const
{
    QString tabs(tabCount*4,' ');
    const static QString singleTab = "    ";
    QString str = tabs + "SettingGroup: " + m_name + "\n" + tabs + "{";

    for(auto const &param : m_settings)
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
    emit settingValueChanged(setting);
}
void SettingGroup::onSettingNameChanged(const QString &)
{
    Setting* setting = qobject_cast<Setting*>(QObject::sender());
    if(!setting)
    {
        SETTINGS_WARNING_PRETTY<< "receiving signal but sender is nullptr?!";
        return;
    }
    emit settingNameChanged(setting);
}
void SettingGroup::onSettingDestroyed(QObject *obj)
{
    Setting* setting = dynamic_cast<Setting*>(obj);
    SettingGroup* settingGroup = dynamic_cast<SettingGroup*>(obj);
    if(setting)
    {
        for(size_t i=0; i<m_settings.size(); ++i)
        {
            if(m_settings[i] == setting)
            {
                m_settings.erase(m_settings.begin() + i);
                emit settingDeleted();
                return;
            }
        }
    }
    else if(settingGroup)
    {
        for(size_t i=0; i<m_settingGroups.size(); ++i)
        {
            if(m_settingGroups[i] == settingGroup)
            {
                m_settingGroups.erase(m_settingGroups.begin() + i);
                emit childGroupDeleted();
                return;
            }
        }
    }
}
/*void SettingGroup::onChildGroupNameChanged(const QString &groupName)
{

}*/
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
void SettingGroup::connectSignals(SettingGroup *settings)
{
    if(!settings) return;
    connect(settings,  &Setting::destroyed,    this,  &SettingGroup::onSettingDestroyed);
}
void SettingGroup::disconnectSignals(SettingGroup *settings)
{
    if(!settings) return;
    disconnect(settings,  &Setting::destroyed,    this,  &SettingGroup::onSettingDestroyed);
}
void SettingGroup::connectSignals()
{
    for(size_t i=0; i<m_settingGroups.size(); ++i)
    {
        connectSignals(m_settingGroups[i]);
    }

    for(size_t i=0; i<m_settings.size(); ++i)
    {
        connectSignals(m_settings[i]);
    }
}
void SettingGroup::disconnectSignals()
{
    for(size_t i=0; i<m_settingGroups.size(); ++i)
    {
        disconnectSignals(m_settingGroups[i]);
    }

    for(size_t i=0; i<m_settings.size(); ++i)
    {
        disconnectSignals(m_settings[i]);
    }
}
Setting *SettingGroup::createSettingInstance()
{
    return new Setting();
}
}
