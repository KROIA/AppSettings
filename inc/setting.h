#pragma once
/**
 * \author  Alex Krieg
 * \version 00.00.00
 * \date    04.10.2022
 *
 * \brief This class is used to store a name with a value.
 *
 */

#include <QObject>
#include <QVariant>
#include <utility>
#include "settingsDeclaration.h"


namespace Settings
{
    class Setting  :   public QObject
    {
            Q_OBJECT

        public:

            /**
             * \brief Constructor
             * \param parent, The parent Settings object, this setting is
             *                contained in
             */
            Setting(Settings *parent = nullptr);

            /**
             * \brief Copy constructor
             * \param other, The other object from which will be copied
             * \param parent, The parent Settings object, this setting is
             *                contained in
             */
            Setting(const Setting &other, Settings *parent = nullptr);

            /**
             * \brief Constructor
             * \param name,  The name of the setting
             * \param value, The value of the parameter, can by any QVariant compatible type
             * \param parent, The parent Settings object, this setting is
             *                contained in
             */
            Setting(const QString &name, const QVariant value, Settings *parent = nullptr);

            /**
             * \brief Constructor
             * \param setting,  a pair of name and value
             * \param parent, The parent Settings object, this setting is
             *                contained in
             */
            Setting(const std::pair<QString,QVariant> &setting, Settings *parent = nullptr);

            ~Setting();

            void setParent(Settings *parent);
            Settings* getParent() const;

            /**
             * \brief assignement operator
             * \param other,  Will copy <other> to <this>
             *                Name and value will be replaced by <other>'s name and value
             */
            const Setting &operator=(const Setting &other);

            /**
             * \brief assignement operator
             * \param value,  Will assign <value> to this settings value
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

        private:
            std::pair<QString,QVariant> m_parameter;
            Settings *m_parent;
    };

}
Q_DECLARE_METATYPE(Settings::Setting);
