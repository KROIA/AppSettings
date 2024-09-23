#include "ApplicationSettings.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include "Utilities.h"

namespace AppSettings
{
	const QString ApplicationSettings::s_fileEnding = ".json";

	ApplicationSettings::ApplicationSettings(const QString& name)
		: m_name(name)
		, m_path("")
	{
		getInstances().push_back(this);
	}
	ApplicationSettings::ApplicationSettings(const QString& path, const QString& name)
		: m_name(name)
		, m_path(path)
	{
		getInstances().push_back(this);
	}
	ApplicationSettings::~ApplicationSettings()
	{
		auto& instances = getInstances();
		for (size_t i = 0; i < instances.size(); ++i)
		{
			if (instances[i] == this)
			{
				instances.erase(instances.begin() + i);
				break;
			}
		}
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
		if(m_path.size() > 0)
			return m_path + "\\" + m_name + s_fileEnding;
		return m_name + s_fileEnding;
	}

	bool ApplicationSettings::save() const
	{
		emit saveStarted();
		bool success = true;
		QString filePath = getFilePath();

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
				Logger::logError("Failed to create directory: \"" + m_path.toStdString() + "\"");
				success = false;
				goto exit;
			}
			else
			{
				Logger::logInfo("Created directory: \"" + m_path.toStdString() + "\"");
			}
		}

		// Write the QJsonObject to a file
		{
			
			QFile file(filePath);
			if (!file.open(QIODevice::WriteOnly))
			{
				Logger::logError("Failed to open file: \"" + filePath.toStdString() + "\"");
				success = false;
				goto exit;
			}
			{
				QJsonDocument doc(settings);
				if (file.write(doc.toJson()) == -1)
				{
					Logger::logError("Failed to write to file: \"" + filePath.toStdString() + "\"");
					success = false;
				}
			}
			file.close();
		}

		exit:
		emit saveFinished(success);
		if(success)
			Logger::logInfo("Settings saved successfully: \"" + m_name.toStdString() + "\" to \""+ filePath.toStdString()+"\"");
		else
			Logger::logError("Failed to save settings: \"" + m_name.toStdString() + "\" to \""+ filePath.toStdString()+"\"");
		return success;
	}
	QJsonObject ApplicationSettings::saveToJson() const
	{
		QJsonObject settings;
		save_internal(settings);
		return settings;
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
			Logger::logError("Failed to open file: \"" + filePath.toStdString() + "\"");
			success = false;
			goto exit;
		}

		{
			QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
			file.close();
			if (!doc.isObject())
			{
				Logger::logError("Json data is not a JsonObject: \"" + filePath.toStdString() + "\"");
				success = false;
				goto exit;
			}
			QJsonObject reader = doc.object();

			// Deserialize the QJsonObject
			success &= read_internal(reader);
		}
		
		exit:
		emit loadFinished(success);
		if(success)
			Logger::logInfo("Settings loaded successfully: \"" + m_name.toStdString() + "\" from \""+filePath.toStdString()+"\"");
		else
			Logger::logError("Failed to load settings: \"" + m_name.toStdString() + "\" from \"" + filePath.toStdString() + "\"");
		return success;
	}
	bool ApplicationSettings::loadFromJson(const QJsonObject& reader)
	{
		emit loadStarted();
		bool success = read_internal(reader);
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

	QString ApplicationSettings::toString() const
	{
		QString str = "{ " + m_name + "\n";
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			str += m_groups[i]->toString_internal(4) + "\n";
		}
		str += "}\n";
		return str;
	}
	QDebug operator<<(QDebug debug, const ApplicationSettings& settings)
	{
		debug.nospace() << settings.toString();
		return debug;
	}

	std::ostream& operator<<(std::ostream& stream, const ApplicationSettings& settings)
	{
		stream << settings.toString().toStdString();
		return stream;
	}

	void ApplicationSettings::saveAll()
	{
		auto& instances = getInstances();
		for (size_t i = 0; i < instances.size(); ++i)
		{
			instances[i]->save();
		}
	}
	void ApplicationSettings::loadAll()
	{
		auto& instances = getInstances();
		for (size_t i = 0; i < instances.size(); ++i)
		{
			instances[i]->load();
		}
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
				Logger::logError("Failed to read SettingsGroup with name: \"" + m_groups[i]->getName().toStdString() + "\"");
				success = false;
			}
		}
		return success;
	}

	std::vector<ApplicationSettings*>& ApplicationSettings::getInstances()
	{
		static std::vector<ApplicationSettings*> instances;
		return instances;
	}
}