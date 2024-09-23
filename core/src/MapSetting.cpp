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
	MapSetting::MapSetting(const QString& name, const std::vector<std::pair<QVariant, QVariant>>& map)
		: m_name(name)
	{
		setMap(map);
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
		for (auto otherIt = other.m_map.begin(); otherIt != other.m_map.end(); ++otherIt)
		{
			bool found = false;
			for (auto it = m_map.begin(); it != m_map.end(); ++it)
			{
				if (it->first == otherIt->first)
				{
					if (it->second != otherIt->second)
						return false;
					found = true;
				}
			}
			if (!found)
				return false;
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
		m_map.clear();
		m_map.reserve(other.m_map.size());
		for (size_t i = 0; i < other.m_map.size(); ++i)
		{
			if (!contains(other.m_map[i].first))
			{
				m_map.push_back(other.m_map[i]);
			}
		}
		m_enableAddButton = other.m_enableAddButton;
		m_enableRemoveButton = other.m_enableRemoveButton;
		emit valueChanged();
		return *this;
	}

	void MapSetting::set(const QVariant& key, const QVariant& value)
	{ 
		size_t currentIdx = getIndex(key);
		if (currentIdx != std::string::npos)
			m_map[currentIdx].second = value;
		else
			m_map.push_back(std::make_pair(key, value));
		emit valueChanged(); 
	}
	QVariant& MapSetting::operator[](const QVariant& key)
	{
		size_t currentIdx = getIndex(key);
		if (currentIdx != std::string::npos)
			return m_map[currentIdx].second;

		m_map.push_back(std::make_pair(key, QVariant()));
		emit valueChanged();
		return m_map.back().second;
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
	void MapSetting::setMap(const std::vector<std::pair<QVariant, QVariant>>& map)
	{
		m_map.reserve(map.size());
		for (size_t i = 0; i < map.size(); ++i)
		{
			if (!contains(map[i].first))
			{
				m_map.push_back(map[i]);
			}
		}
		emit valueChanged();
	}
	void MapSetting::addPair(const QVariant& key, const QVariant& value)
	{
		if (!contains(key))
		{
			m_map.push_back(std::make_pair(key, value));
			emit valueChanged();
		}
	}

	void MapSetting::removePair(const QVariant& key)
	{
		for (auto it = m_map.begin(); it != m_map.end(); ++it)
		{
			if (it->first == key)
			{
				m_map.erase(it);
				emit valueChanged();
				return;
			}
		}
	}

	bool MapSetting::contains(const QVariant& key) const
	{
		for (auto it = m_map.begin(); it != m_map.end(); ++it)
		{
			if (it->first == key)
			{
				return true;
			}
		}
		return false;	
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
		QJsonArray mapObject = value.toArray();
		m_map.clear();
		for (int i = 0; i < mapObject.size(); ++i)
		{
			QJsonObject obj = mapObject[i].toObject();
			QVariant key = jsonValueToVariant(obj["Key"]);
			QVariant val = jsonValueToVariant(obj["Val"]);

			size_t currentIdx = getIndex(key);
			if (currentIdx != std::string::npos)
				m_map[currentIdx].second = val;
			else
				m_map.push_back(std::make_pair(key, val));
		}
		emit valueChanged();
		return true;
	}

	size_t MapSetting::getIndex(const QVariant& key) const
	{
		for (size_t i = 0; i < m_map.size(); ++i)
		{
			if (m_map[i].first == key)
			{
				return i;
			}
		}
		return std::string::npos;
	}

}