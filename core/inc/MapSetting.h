#pragma once

#include "AppSettings_base.h"
#include "ISetting.h"
#include <QVariant>
#include <QString>
#include <vector>

namespace AppSettings
{
	/// <summary>
	/// Settings type which holds a map of key-value pairs.
	/// Both key and value are of type QVariant.
	/// </summary>
	class APP_SETTINGS_EXPORT MapSetting : public ISetting
	{
	public:

		MapSetting();
		MapSetting(const MapSetting& other);
		MapSetting(const QString& name);
		MapSetting(const QString& name, const std::vector<std::pair<QVariant, QVariant>>& value);

		bool operator==(const MapSetting& other) const;
		bool operator!=(const MapSetting& other) const;

		MapSetting& operator=(const MapSetting& other);
		QVariant& operator[](const QVariant& key);
		void set(const QVariant& key, const QVariant& value);

		void setName(const QString& name) override { m_name = name; }

		QString getName() const override { return m_name; }
		QString toString() const override;

		void setMap(const std::vector<std::pair<QVariant, QVariant>>& map);
		void addPair(const QVariant& key, const QVariant& value);
		void removePair(const QVariant& key);
		const std::vector<std::pair<QVariant, QVariant>> &getMap() const { return m_map; }
		void clear() { m_map.clear(); }
		bool contains(const QVariant& key) const;
		size_t size() const { return m_map.size(); }
		std::vector<QVariant> getKeys() const;
		std::vector<QVariant> getValues() const;

		bool isAddButtonEnabled() const { return m_enableAddButton; }
		void setAddButtonEnabled(bool enabled) { m_enableAddButton = enabled; }
		bool isRemoveButtonEnabled() const { return m_enableRemoveButton; }
		void setRemoveButtonEnabled(bool enabled) { m_enableRemoveButton = enabled; }

	protected:
		void save(QJsonObject& settings) const override;
		bool load(const QJsonObject& settings) override;

	private:
		size_t getIndex(const QVariant& key) const;
		QString m_name;

		// Can't use std::map because QVariant operator > & < is deprecated 
		// This is a pseudo map, which is actually a vector of pairs
		std::vector<std::pair<QVariant,QVariant>> m_map;

		// Flags to enable/disable the add and remove buttons in the UI
		bool m_enableAddButton = true;
		bool m_enableRemoveButton = true;
	};
}