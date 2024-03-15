#pragma once

#include "AppSettings_base.h"
#include "Setting.h"
#include <vector>
#include <QObject>

namespace AppSettings
{
	class ApplicationSettings;
	/// <summary>
	/// Used to combine multiple settings into a single group.
	/// It can also contain other groups.
	/// 
	/// A tree structure can be created by adding groups to groups.
	/// </summary>
	class APP_SETTINGS_EXPORT SettingsGroup : public QObject, public IJsonSerializable
	{
		friend ApplicationSettings;
		Q_OBJECT
	public:
		/// <summary>
		/// Creates a new group with the given name.
		/// </summary>
		/// <param name="name">Name of the settings group</param>
		SettingsGroup(const QString &name);
		SettingsGroup(const SettingsGroup& other) = delete;
		SettingsGroup(const SettingsGroup&& other) = delete;
		virtual ~SettingsGroup();

		/// <summary>
		/// Changes the name of the settings group
		/// </summary>
		/// <param name="name">Name of the settings group</param>
		void setName(const QString &name);

		/// <summary>
		/// Gets the name of the settings group
		/// </summary>
		/// <returns>The name of the settings group</returns>
		const QString &getName() const;

		/// <summary>
		/// Gets the amount of settings in the settings group. (Not including settings in subgroups)
		/// </summary>
		/// <returns>The amount of settings in the settings group</returns>
		size_t getSettingCount() const;

		/// <summary>
		/// Gets the amount of settings groups in the settings group. (Not including subgroups in subgroups)
		/// </summary>
		/// <returns>The amount of settings groups in the settings group</returns>
		size_t getGroupCount() const;

		/// <summary>
		/// Gets the group with the given name.
		/// </summary>
		/// <param name="name">Name of the group</param>
		/// <returns>The group with the given name or nullptr if it does not exist</returns>
		const SettingsGroup* getGroup(const QString& name) const;

		/// <summary>
		/// Gets the group at the given index.
		/// </summary>
		/// <param name="index">Index of the group</param>
		/// <returns>The group with the given name or nullptr if it does not exist</returns>
		const SettingsGroup* getGroup(size_t index) const;
		
		/// <summary>
		/// Gets the setting with the given name.
		/// </summary>
		/// <param name="name">Name of the setting</param>
		/// <returns>The setting with the given name or nullptr if it does not exist</returns>
		const Setting* getSetting(const QString& name) const;

		/// <summary>
		/// Gets the setting at the given index.
		/// </summary>
		/// <param name="index">Index of the setting</param>
		/// <returns>The setting with the given name or nullptr if it does not exist</returns>
		const Setting* getSetting(size_t index) const;

	signals:
		/// <summary>
		/// Signal emitted when the name of the settings group is changed
		/// </summary>
		/// <param name="name">New name of the settings group</param>
		void nameChanged(const QString& name);

	protected:
		/// <summary>
		/// Adds a setting to the settings group.
		/// </summary>
		/// <param name="setting">Setting to add to the settings group</param>
		void addSetting(Setting& setting);

		/// <summary>
		/// Adds a settings group to the settings group.
		/// </summary>
		/// <param name="group">Group to add to the settings group</param>
		void addGroup(SettingsGroup& group);

		/// <summary>
		/// Saves the setting group to the given json object.
		/// </summary>
		/// <param name="settings">QJsonObject container to save the settings</param>
		void save(QJsonObject& settings) const override;

		/// <summary>
		/// Loads the settings back from the given json object.
		/// </summary>
		/// <param name="reader">QJsonObject container to load the settings</param>
		/// <returns></returns>
		bool load(const QJsonObject& reader) override;
	private:
		
		/// <summary>
		/// Gets the key of the settings group, used to save the settings to a json object.
		/// </summary>
		/// <returns>A string containing the name and _Group: name_Group</returns>
		QString getGroupKey() const;

		QString m_name;
		std::vector<Setting*> m_settings;
		std::vector<SettingsGroup*> m_groups;
	};
}