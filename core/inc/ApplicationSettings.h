#pragma once

#include "Settings_base.h"
#include "SettingsGroup.h"

namespace Settings
{
	class SETTINGS_EXPORT ApplicationSettings
	{
	public:
		// Name of the application, will be the name of the settings file
		ApplicationSettings(const QString& name);

		// Path to the settings folder, will be the path of the settings file
		// Name of the application, will be the name of the settings file
		ApplicationSettings(const QString &path, const QString &name);
		
		ApplicationSettings(const ApplicationSettings& other) = delete;
		ApplicationSettings(const ApplicationSettings&& other) = delete;
		virtual ~ApplicationSettings();

		void setName(const QString &name); // Name of the application
		void setPath(const QString &path); // Path to the settings folder

		const QString &getName() const;
		const QString &getPath() const;

		QString getFilePath() const;

		bool save() const;
		bool read();

	protected:
		void addGroup(SettingsGroup& group);

	private:
		void save_internal(QJsonObject& settings) const;
		bool read_internal(const QJsonObject& reader);


		QString m_name;
		QString m_path;
		std::vector<SettingsGroup*> m_groups;

		const static QString s_fileEnding;
	};
}