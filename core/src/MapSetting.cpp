#include "MapSetting.h"
#include "Utilities.h"
#include <QJsonArray>

namespace AppSettings
{
	MapSetting::MapSetting()
		: m_name("MapSetting")
	{

	}
	MapSetting::MapSetting(const MapSetting& other)
		: m_name(other.m_name)
		, m_map(other.m_map)
	{

	}
	MapSetting::MapSetting(const QString& name)
		: m_name(name)
	{

	}
	MapSetting::MapSetting(const QString& name, const std::map<QVariant, QVariant>& map)
		: m_name(name)
		, m_map(map)
	{

	}

	bool MapSetting::operator==(const MapSetting& other) const
	{
		if (m_name != other.m_name)
		{
			return false;
		}
		if (m_map.size() != other.m_map.size())
		{
			return false;
		}
		for (auto it = m_map.begin(); it != m_map.end(); ++it)
		{
			auto otherIt = other.m_map.find(it->first);
			if (otherIt == other.m_map.end())
			{
				return false;
			}
			if (it->second != otherIt->second)
			{
				return false;
			}
		}
		return true;
	}
	bool MapSetting::operator!=(const MapSetting& other) const
	{
		return !(*this == other);
	}

	MapSetting& MapSetting::operator=(const MapSetting& other)
	{
		m_name = other.m_name;
		m_map = other.m_map;
		m_enableAddButton = other.m_enableAddButton;
		m_enableRemoveButton = other.m_enableRemoveButton;
		emit valueChanged();
		return *this;
	}

	void MapSetting::set(const QVariant& key, const QVariant& value)
	{ 
		m_map[key] = value; 
		emit valueChanged(); 
	}

	QString MapSetting::toString() const
	{
		QString str = QString("MapSetting(%1) {").arg(m_name);
		for (auto it = m_map.begin(); it != m_map.end(); ++it)
		{
			str += QString("\n\t%1: %2").arg(it->first.toString()).arg(it->second.toString());
		}
		str += "\n}";
		return str;
	}

	std::vector<QVariant> MapSetting::getKeys() const
	{
		std::vector<QVariant> keys;
		for (auto it = m_map.begin(); it != m_map.end(); ++it)
		{
			keys.push_back(it->first);
		}
		return keys;
	
	}
	std::vector<QVariant> MapSetting::getValues() const
	{
		std::vector<QVariant> values;
		for (auto it = m_map.begin(); it != m_map.end(); ++it)
		{
			values.push_back(it->second);
		}
		return values;
	}

	void MapSetting::save(QJsonObject& settings) const
	{
		QJsonArray mapObject;
		for (auto it = m_map.begin(); it != m_map.end(); ++it)
		{
			QJsonObject obj;
			obj["Key"] = variantToJsonValue(it->first);
			obj["Val"] = variantToJsonValue(it->second);
			mapObject.append(obj);
		}
		settings[m_name] = mapObject;
	}
	bool MapSetting::load(const QJsonObject& settings)
	{
		if (!settings.contains(m_name))
		{
			logger().logError("Unable to read setting: " + m_name.toStdString()
				+ ". Setting not found");
			return false;
		}
		QJsonValue value = settings[m_name];
		if (!value.isArray())
		{
			logger().logError("Unable to read setting: " + m_name.toStdString()
				+ ". Setting is not an array");
			return false;
		}
		QJsonArray mapObject = value.toArray();
		m_map.clear();
		for (int i = 0; i < mapObject.size(); ++i)
		{
			QJsonObject obj = mapObject[i].toObject();
			QVariant key = jsonValueToVariant(obj["Key"]);
			QVariant val = jsonValueToVariant(obj["Val"]);
			m_map[key] = val;
		}
		emit valueChanged();
		return true;
	}

}