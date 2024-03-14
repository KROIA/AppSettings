#pragma once

#include "AppSettings_base.h"
#include "IJsonSerializable.h"
#include <QObject>
#include <QVariant>
#include <utility>


namespace AppSettings
{

    /**
     * \author  Alex Krieg
     * \version 00.01.00
     * \date    21.11.2023
     *
     * \brief This class is used to store a name with a value.
     *
     */
    class APP_SETTINGS_EXPORT Setting: public QObject, public IJsonSerializable
    {
            Q_OBJECT

        public:

            /**
             * \brief Constructor
             */
            Setting();

            /**
             * \brief Copy constructor
             * \param other, The other object from which will be copied
             */
            Setting(const Setting &other);

            /**
             * \brief Constructor
             * \param name,  The name of the setting
             * \param value, The value of the parameter, can by any QVariant compatible type
             */
            Setting(const QString &name, const QVariant &value);

            /**
             * \brief Constructor
             * \param setting,  a pair of name and value
             */
            Setting(const std::pair<QString,QVariant> &setting);

            virtual ~Setting();

            /**
             * @brief operator ==
             * @param other parameter to compare the value to this value
             * @return true if both setting values are equal. Not comparing the name of the setting
             */
            bool operator==(const Setting& other);

            /**
             * @see operator==()
             */
            bool operator==(const QVariant& otherValue);

            /**
             * @brief operator !=
             * @param other parameter to compare the value to this value
             * @return true if both setting values are not equal. Not comparing the name of the setting
             */
            bool operator!=(const Setting& other);

            /**
             * @see operator!=()
             */
            bool operator!=(const QVariant& otherValue);

            /**
             * \brief assignement operator
             * \param other,  Will copy <other> to <this>
             *                Name and value will be replaced by <other>'s name and value
             */
            const Setting &operator=(const Setting &other);

            /**
             * \brief assignement operator
             * \param value,  Will assign <value> to this SettingGroup value
             */
            const Setting &operator=(const QVariant &value);

            /**
             * \brief getValue
             * \return returns the value of the setting as a QVariant
             */
            const QVariant &getValue() const;

            /**
             * \brief getName
             * \return returns the value of the setting as a QVariant
             */
            const QString &getName() const;

            /**
             * \brief getPair
             * \return returns the setting as a pair object
             */
            const std::pair<QString,QVariant> &getPair() const;


            /**
             * \brief toString
             * \return return the setting as a string in form of:
             *         "{ <name> = <value> }";
             */
            QString toString() const;

            /**
             * \brief debug operator
             *        Prints the setting to the QT-debug console in the same form as this->toString()
             * \see toString()
             *        Usage:
             *        Setting mySetting();
             *        qDebug() << mySetting;
             */
            friend QDebug operator<<(QDebug debug, const Setting &setting);



            void save(QJsonObject& settings) const override;
            bool read(const QJsonObject& reader) override;

        signals:

            /**
             * \brief valueChanged
             * \details Will be emitted if the setting value has been changed
             */
            void valueChanged(const QVariant &value);

            /**
             * \brief nameChanged
             * \details Will be emitted if the setting name has been changed
             */
            void nameChanged(const QString &name);

        public slots:

            /**
             * \brief setValue
             * \details Will set the value of the setting
             * \param value, The value of the parameter, can by any QVariant compatible type
             */
            void setValue(const QVariant &value);

            /**
             * \brief setName
             * \details Will set the name of the setting
             * \param name,  The name of the setting
             */
            void setName(const QString &name);

        protected:


            std::pair<QString,QVariant> m_parameter;
    };

}
