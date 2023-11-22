#include "SettingsGroup.h"

namespace Settings
{
	SettingsGroup::SettingsGroup(const QString& name)
		: m_name(name)
	{
		m_settings.reserve(50);
	}
	SettingsGroup::~SettingsGroup()
	{

	}

	void SettingsGroup::setName(const QString& name)
	{
		m_name = name;
	}
	const QString& SettingsGroup::getName() const
	{
		return m_name;
	}

	void SettingsGroup::save(QJsonObject& settings) const
	{
		QJsonObject group;
		for (size_t i = 0; i < m_settings.size(); ++i)
		{
			m_settings[i]->save(group);
		}
		

		
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			//QJsonObject subGroup;
			m_groups[i]->save(group);
			//group[m_groups[i]->m_name+"Group"] = std::move(subGroup);
		}
		settings[getGroupKey()] = std::move(group);
		
	}
	bool SettingsGroup::read(const QJsonObject& reader)
	{
		if (!reader.contains(getGroupKey()))
		{
			SETTINGS_WARNING_PRETTY << "No SettingsGroup with name: \"" << m_name << "\" found";
			return false;
		}
		QJsonObject group = reader[getGroupKey()].toObject();
		bool success = true;
		for (size_t i = 0; i < m_settings.size(); ++i)
		{
			if (!m_settings[i]->read(group))
			{
				SETTINGS_WARNING_PRETTY << "Failed to read Setting with name: \"" << m_settings[i]->getName() << "\"";
				success = false;
			}
		}
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			//QJsonObject subGroup = group[m_groups[i]->m_name + "Group"].toObject();
			success &= m_groups[i]->read(group);
		}
		return success;
	}

	void SettingsGroup::addSetting(Setting& setting)
	{
		m_settings.push_back(&setting);
	}
	void SettingsGroup::addGroup(SettingsGroup& group)
	{
		m_groups.push_back(&group);
	}

	QString SettingsGroup::getGroupKey() const
	{
		return m_name + "_Group";
	}

}