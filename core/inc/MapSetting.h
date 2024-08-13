#pragma once

#include "AppSettings_base.h"
#include "ISetting.h"
#include <QVariant>
#include <QString>
#include <map>
#include <vector>

namespace AppSettings
{
	class APP_SETTINGS_EXPORT MapSetting : public ISetting
	{
	public:

		MapSetting();
		MapSetting(const MapSetting& other);
		MapSetting(const QString& name);
		MapSetting(const QString& name, const std::map<QVariant,QVariant>& value);

		bool operator==(const MapSetting& other) const;
		bool operator!=(const MapSetting& other) const;

		MapSetting& operator=(const MapSetting& other);
		QVariant operator[](const QVariant& key) const { return m_map.at(key); }
		void set(const QVariant& key, const QVariant& value) { m_map[key] = value; emit valueChanged(); }

		void setName(const QString& name) override { m_name = name; }

		QString getName() const override { return m_name; }
		QString toString() const override;

		void setMap(const std::map<QVariant,QVariant>& map) { m_map = map; }
		void addPair(const QVariant& key, const QVariant& value) { m_map[key] = value; }
		void removePair(const QVariant& key) { m_map.erase(key); }
		const std::map<QVariant,QVariant> &getMap() const { return m_map; }
		void clear() { m_map.clear(); }
		bool contains(const QVariant& key) const { return m_map.find(key) != m_map.end(); }
		size_t size() const { return m_map.size(); }
		std::vector<QVariant> keys() const;
		std::vector<QVariant> values() const;

	protected:
		void save(QJsonObject& settings) const override;
		bool load(const QJsonObject& settings) override;

	private:
		QString m_name;
		std::map<QVariant,QVariant> m_map;
	};
}