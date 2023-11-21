#pragma once

#include "Settings_base.h"
#include "Setting.h"
#include <vector>

namespace Settings
{
	class SETTINGS_EXPORT SettingsGroup : public IJsonSerializable
	{
	public:
		SettingsGroup(const QString &name);
		SettingsGroup(const SettingsGroup& other) = delete;
		SettingsGroup(const SettingsGroup&& other) = delete;
		virtual ~SettingsGroup();

		void setName(const QString &name);
		const QString &getName() const;

		void save(QJsonObject& settings) const override;
		bool read(const QJsonObject& reader) override;

	protected:
		void addSetting(Setting& setting);

	private:

		QString m_name;
		std::vector<Setting*> m_settings;

	};
}