#include "ApplicationSettings.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>

namespace Settings
{
	const QString ApplicationSettings::s_fileEnding = ".json";

	ApplicationSettings::ApplicationSettings(const QString& name)
		: m_name(name)
		, m_path("")
	{
	}
	ApplicationSettings::ApplicationSettings(const QString& path, const QString& name)
		: m_name(name)
		, m_path(path)
	{
	}
	ApplicationSettings::~ApplicationSettings()
	{
	}
	
	
	void ApplicationSettings::setName(const QString& name)
	{
		m_name = name;
	}
	void ApplicationSettings::setPath(const QString& path)
	{
		m_path = path;
	}

	const QString& ApplicationSettings::getName() const
	{
		return m_name;
	}
	const QString& ApplicationSettings::getPath() const
	{
		return m_path;
	}
	QString ApplicationSettings::getFilePath() const
	{
		return m_path + "\\" + m_name + s_fileEnding;
	}

	bool ApplicationSettings::save() const
	{
		// Serialize a QJsonObject with all the settings
		QJsonObject settings;
		save_internal(settings);

		// Check if the directory exists
		QDir dir(m_path);
		if (!dir.exists())
		{
			QDir dir2;
			if (!dir2.mkpath(m_path))
			{
				SETTINGS_WARNING_PRETTY << "Failed to create directory: \"" << m_path << "\"";
				return false;
			}
		}

		// Write the QJsonObject to a file
		QString filePath = getFilePath();
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly))
		{
			SETTINGS_WARNING_PRETTY << "Failed to open file: \"" << filePath << "\"";
			return false;
		}
		QJsonDocument doc(settings);
		file.write(doc.toJson());
		file.close();
		return true;
	}
	bool ApplicationSettings::read()
	{
		// Read the JsonObject from a file
		QString filePath = getFilePath();
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly))
		{
			SETTINGS_WARNING_PRETTY << "Failed to open file: \"" << filePath << "\"";
			return false;
		}
		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		file.close();
		QJsonObject reader = doc.object();

		// Deserialize the QJsonObject
		return read_internal(reader);
	}

	void ApplicationSettings::addGroup(SettingsGroup& group)
	{
		m_groups.push_back(&group);
	}


	void ApplicationSettings::save_internal(QJsonObject& settings) const
	{
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			m_groups[i]->save(settings);
		}
	}
	bool ApplicationSettings::read_internal(const QJsonObject& reader)
	{
		bool success = true;
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			if (!m_groups[i]->read(reader))
			{
				SETTINGS_WARNING_PRETTY << "Failed to read SettingsGroup with name: \"" << m_groups[i]->getName() << "\"";
				success = false;
			}
		}
		return success;
	}
}