#include "ApplicationSettings.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>

namespace AppSettings
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
		emit nameChanged(m_name);
	}
	void ApplicationSettings::setPath(const QString& path)
	{
		m_path = path;
		emit pathChanged(m_path);
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
		emit saveStarted();
		bool success = true;
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
				AS_CONSOLE_FUNCTION("Failed to create directory: \"" << m_path.toStdString() << "\"");
				success = false;
				goto exit;
			}
		}

		// Write the QJsonObject to a file
		{
			QString filePath = getFilePath();
			QFile file(filePath);
			if (!file.open(QIODevice::WriteOnly))
			{
				AS_CONSOLE_FUNCTION("Failed to open file: \"" << filePath.toStdString() << "\"");
				success = false;
				goto exit;
			}
			{
				QJsonDocument doc(settings);
				if (file.write(doc.toJson()) == -1)
				{
					AS_CONSOLE_FUNCTION("Failed to write to file: \"" << filePath.toStdString() << "\"");
					success = false;
				}
			}
			file.close();
		}

		exit:
		emit saveFinished(success);
		return success;
	}
	bool ApplicationSettings::load()
	{
		emit loadStarted();
		bool success = true;
		// Read the JsonObject from a file
		QString filePath = getFilePath();
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly))
		{
			AS_CONSOLE_FUNCTION("Failed to open file: \"" << filePath.toStdString() << "\"");
			success = false;
			goto exit;
		}

		{
			QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
			file.close();
			if (!doc.isObject())
			{
				AS_CONSOLE_FUNCTION("Json data is not a JsonObject: \"" << filePath.toStdString() << "\"");
				success = false;
				goto exit;
			}
			QJsonObject reader = doc.object();

			// Deserialize the QJsonObject
			success &= read_internal(reader);
		}
		
		exit:
		emit loadFinished(success);
		return success;
	}

	const SettingsGroup* ApplicationSettings::getGroup(const QString& name) const
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

	const SettingsGroup* ApplicationSettings::getGroup(size_t index) const
	{
		if (index < m_groups.size())
		{
			return m_groups[index];
		}
		return nullptr;
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
			if (!m_groups[i]->load(reader))
			{
				AS_CONSOLE_FUNCTION("Failed to read SettingsGroup with name: \"" << m_groups[i]->getName().toStdString() << "\"");
				success = false;
			}
		}
		return success;
	}
}