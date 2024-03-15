#pragma once

#include "AppSettings_base.h"
#include "SettingsGroup.h"

namespace AppSettings
{
	/// <summary>
	/// This class will be the main class for the application settings in a project.
	/// It will be used to save and load the settings of the application.
	/// </summary>
	class APP_SETTINGS_EXPORT ApplicationSettings: public QObject
	{
		Q_OBJECT
	public:
		/// <summary>
		/// Creates a new instance of the ApplicationSettings class
		/// </summary>
		/// <param name="name">The name of the settings file</param>
		ApplicationSettings(const QString& name);

		/// <summary>
		/// Creates a new instance of the ApplicationSettings class
		/// </summary>
		/// <param name="path">The folderpath where the settings file will be stored in</param>
		/// <param name="name">The name of the settings file</param>
		ApplicationSettings(const QString &path, const QString &name);
		
		ApplicationSettings(const ApplicationSettings& other) = delete;
		ApplicationSettings(const ApplicationSettings&& other) = delete;
		virtual ~ApplicationSettings();

		/// <summary>
		///	Sets the name of the settings file
		/// </summary>
		/// <param name="name"></param>
		void setName(const QString &name);

		/// <summary>
		/// Sets the path to the settings folder
		/// </summary>
		/// <param name="path"></param>
		void setPath(const QString &path);

		/// <summary>
		/// 
		/// </summary>
		/// <returns>The name of the settings file</returns>
		const QString &getName() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns>The path to the settings folder</returns>
		const QString &getPath() const;

		/// <summary>
		/// Gets the full file path to the settings file, including the file name and file ending
		/// </summary>
		/// <returns>The full path to the settings file</returns>
		QString getFilePath() const;

		/// <summary>
		/// Saves the settings to the filesystem
		/// </summary>
		/// <returns>True, if saving was successfull, otherwise false </returns>
		bool save() const;

		/// <summary>
		/// Loads the settings from the filesystem
		/// </summary>
		/// <returns>True, if the file was loaded successfully</returns>
		bool load();

	signals:

		/// <summary>
		/// Signal that is emitted when the name of the settings file has changed
		/// </summary>
		/// <param name="name">The new name of the settings file</param>
		void nameChanged(const QString& name) const;

		/// <summary>
		/// Signal that is emitted when the path to the settings folder has changed
		/// </summary>
		/// <param name="path">The new path to the settings folder</param>
		void pathChanged(const QString& path) const;
		
		/// <summary>
		/// Signal that is emitted when the saving of the settings has started
		/// </summary>
		void saveStarted() const;

		/// <summary>	
		/// Signal that is emitted when the saving of the settings has finished
		/// </summary>
		/// <param name="success">True, if the saving was successful, otherwise false</param>
		void saveFinished(bool success) const;

		/// <summary>
		/// Signal that is emitted when the loading of the settings has started
		/// </summary>
		void loadStarted() const;

		/// <summary>
		/// Signal that is emitted when the loading of the settings has finished
		/// </summary>
		/// <param name="success">True, if the loading was successful, otherwise false</param>
		void loadFinished(bool success) const;

	protected:
		/// <summary>
		/// Adds a new settings group to the application settings
		/// </summary>
		/// <param name="group">Settings group to be added</param>
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