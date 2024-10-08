#pragma once

#include "AppSettings_base.h"
#include "ISetting.h"
#include <QObject>
#include <QVariant>
#include <utility>



namespace AppSettings
{
    class SettingsGroup;
    /// <summary>
    /// This class is a simple container for a setting.
    /// It contains a name and a value.
    /// The name is a QString and the value is a QVariant.
    /// </summary>
    class APP_SETTINGS_EXPORT Setting: public ISetting
    {
        friend SettingsGroup;
            
        public:

            Setting();
            Setting(const Setting &other);

            /// <summary>
            /// Constructor
            /// </summary>
            /// <param name="name">The name of the setting</param>
            /// <param name="value">The value of the parameter, can by any QVariant compatible type</param>
            Setting(const QString &name, const QVariant &value);

            /// <summary>
            /// Constructor
            /// </summary>
            /// <param name="setting">a pair of name and value</param>
            Setting(const std::pair<QString,QVariant> &setting);

            virtual ~Setting();

            /// <summary>
            /// operator ==
            /// </summary>
            /// <param name="other">setting to compare with</param>
            /// <returns>True, if both setting values are equal. Not comparing the name of the setting</returns>
            bool operator==(const Setting& other);

            /// <summary>
            /// operator ==
            /// </summary>
            /// <param name="otherValue">value to compare with</param>
            /// <returns>True, if both setting values are equal</returns>
            bool operator==(const QVariant& otherValue);

            /// <summary>
            /// operator ==
            /// </summary>
            /// <param name="otherValue">setting to compare with</param>
            /// <returns>True, if both setting values are not equal. Not comparing the name of the setting</returns>
            bool operator!=(const Setting& other);

            /// <summary>
            /// operator ==
            /// </summary>
            /// <param name="otherValue">value to compare with</param>
            /// <returns>True, if both setting values are not equal</returns>
            bool operator!=(const QVariant& otherValue);

            /// <summary>
            /// Assignement operator
            /// Name and value will be replaced by other's name and value
            /// </summary>
            /// <param name="other">Copies the value from the other setting to this setting</param>
            /// <returns>returns a reference to this</returns>
            Setting &operator=(const Setting &other);

            /// <summary>
            /// Assignement operator
            /// Value will be replaced by value's name and value
            /// </summary>
            /// <param name="value">Copies the value to this setting</param>
            /// <returns>returns a reference to this</returns>
            Setting &operator=(const QVariant &value);

            /// <summary>
            /// Gets the value of the setting
            /// </summary>
            /// <returns>The value of the setting</returns>
            const QVariant &getValue() const;

            /// <summary>
            /// Gets the name of the setting
            /// </summary>
            /// <returns>The name of the setting</returns>
            QString getName() const override;

            /// <summary>
            /// Gets the setting as a pair object
            /// </summary>
            /// <returns>the setting as a pair object</returns>
            const std::pair<QString,QVariant> &getPair() const;

            /// <summary>
            /// Serializes the setting to a string with the format:
            /// "name = value"
            /// </summary>
            /// <returns>The serialized name-value pair</returns>
            QString toString() const override;

            /// <summary>
            /// Sets the value of the setting
            /// </summary>
            /// <param name="value">The new value of this setting</param>
            void setValue(const QVariant &value);

            /// <summary>
            /// Sets the name of the setting
            /// </summary>
            /// <param name="name">The new name of this setting</param>
            void setName(const QString &name) override;

        protected:

            /// <summary>
            /// Saves the setting to a QJsonObject
            /// </summary>
            /// <param name="settings">The QJsonObject to save the setting to</param>
            void save(QJsonObject& settings) const override;

            /// <summary>
            /// Reads the setting from a QJsonObject
            /// </summary>
            /// <param name="reader">The QJsonObject to read the setting from</param>
            /// <returns>True, if the setting could be read</returns>
            /// <returns>False, if the setting could not be read</returns>
            bool load(const QJsonObject& reader) override;

        private:
             


            std::pair<QString,QVariant> m_parameter;
    };

}
