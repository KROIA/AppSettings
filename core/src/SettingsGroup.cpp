#include "SettingsGroup.h"

namespace AppSettings
{
	SettingsGroup::SettingsGroup(const QString& name)
		: QObject()
		, IJsonSerializable()
		, m_name(name)
	{
		m_settings.reserve(50);
	}
	SettingsGroup::~SettingsGroup()
	{

	}

	void SettingsGroup::setName(const QString& name)
	{
		m_name = name;
		emit nameChanged(m_name);
	}
	const QString& SettingsGroup::getName() const
	{
		return m_name;
	}
	size_t SettingsGroup::getSettingCount() const
	{
		return m_settings.size();
	}
	size_t SettingsGroup::getGroupCount() const
	{
		return m_groups.size();
	}

	const SettingsGroup* SettingsGroup::getGroup(const QString& name) const
	{
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			if (m_groups[i]->getName() == name)
			{
				return m_groups[i];
			}
		}
		return nullptr;
	}
	const SettingsGroup* SettingsGroup::getGroup(size_t index) const
	{
		if (index < m_groups.size())
		{
			return m_groups[index];
		}
		return nullptr;
	}
	const Setting* SettingsGroup::getSetting(const QString& name) const
	{
		for (size_t i = 0; i < m_settings.size(); ++i)
		{
			if (m_settings[i]->getName() == name)
			{
				return m_settings[i];
			}
		}
		return nullptr;
	}
	const Setting* SettingsGroup::getSetting(size_t index) const
	{
		if (index < m_settings.size())
		{
			return m_settings[index];
		}
		return nullptr;
	}

	QString SettingsGroup::toString() const
	{
		return toString_internal(0)+"\n";
	}
	QString SettingsGroup::toString_internal(int tabs) const
	{
		QString str = QString(tabs, ' ')+"{ " + m_name + "\n";
		tabs += 4;
		for (size_t i = 0; i < m_settings.size(); ++i)
		{
			str += QString(tabs,' ') + "[" + QString::number(i) + "] " + m_settings[i]->toString() + "\n";
		}
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			str += m_groups[i]->toString_internal(tabs) + "\n";
		}
		str += QString(tabs-4, ' ') + "}";
		return str;
	}

	QDebug operator<<(QDebug debug, const SettingsGroup& setting)
	{
		debug.nospace() << setting.toString();
		return debug;
	}
	std::ostream& operator<<(std::ostream& stream, const SettingsGroup& setting)
	{
		stream << setting.toString().toStdString();
		return stream;
	}


	void SettingsGroup::addSetting(Setting& setting)
	{
		m_settings.push_back(&setting);
	}
	void SettingsGroup::addGroup(SettingsGroup& group)
	{
		m_groups.push_back(&group);
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
			m_groups[i]->save(group);
		}
		settings[getGroupKey()] = std::move(group);
	}
	bool SettingsGroup::load(const QJsonObject& reader)
	{
		if (!reader.contains(getGroupKey()))
		{
			AS_CONSOLE_FUNCTION("No SettingsGroup with name: \"" << m_name.toStdString() << "\" found");
			return false;
		}
		QJsonObject group = reader[getGroupKey()].toObject();
		bool success = true;
		for (size_t i = 0; i < m_settings.size(); ++i)
		{
			if (!m_settings[i]->load(group))
			{
				AS_CONSOLE_FUNCTION("Failed to read Setting with name: \"" << m_settings[i]->getName().toStdString() << "\"");
				success = false;
			}
		}
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			success &= m_groups[i]->load(group);
		}
		return success;
	}

	QString SettingsGroup::getGroupKey() const
	{
		return m_name + "_Group";
	}

}