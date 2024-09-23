#include "ListSetting.h"
#include <QJsonArray>
#include "Utilities.h"

namespace AppSettings
{

	ListSetting::ListSetting()
		: m_name("ListSetting")
	{

	}

	ListSetting::ListSetting(const ListSetting& other)
		: m_name(other.m_name)
		, m_list(other.m_list)
	{

	}
	ListSetting::ListSetting(const QString& name)
		: m_name(name)
	{

	}
	ListSetting::ListSetting(const QString& name, const std::vector<QVariant>& list)
		: m_name(name)
		, m_list(list)
	{

	}

	bool ListSetting::operator==(const ListSetting& other)
	{
		if(m_name != other.m_name)
			return false;
		if (m_list.size() != other.m_list.size())
			return false;
		return std::equal(m_list.begin(), m_list.end(), other.m_list.begin());
	}
	bool ListSetting::operator!=(const ListSetting& other)
	{
		return !(*this == other);
	}

	ListSetting& ListSetting::operator=(const ListSetting& other)
	{
		m_name = other.m_name;
		m_list = other.m_list;
		emit valueChanged();
		return *this;
	}

	QString ListSetting::toString() const
	{
		QString buff = m_name + " [\n";
		for (const QVariant& var : m_list)
		{
			buff += var.toString() + "\n";
		}
		buff += "]";
		return buff;
	}

	void ListSetting::save(QJsonObject& settings) const
	{
		QJsonArray list;
		for (const QVariant& var : m_list)
		{
			list.append(variantToJsonValue(var));
		}
		settings[m_name] = list;
	}
	bool ListSetting::load(const QJsonObject& settings)
	{
		if (!settings.contains(m_name))
		{
			Logger::logError("Unable to read setting: " + m_name.toStdString()
				+ ". Setting not found");
			return false;
		}
		QJsonValue value = settings[m_name];
		if (!value.isArray())
		{
			Logger::logError("Unable to read setting: " + m_name.toStdString()
				+ ". Setting is not an array");
			return false;
		}
		QJsonArray list = value.toArray();
		std::vector<QVariant> oldList = m_list;
		m_list.clear();
		m_list.reserve(list.size());
		for (const QJsonValue& val : list)
		{
			m_list.push_back(jsonValueToVariant(val));
		}
		if (m_list != oldList)
		{
			emit valueChanged();
		}
		return true;
	}

	
}