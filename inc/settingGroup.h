#pragma once
/**
 * \author  Alex Krieg
 * \version 00.00.00
 * \date    04.10.2022
 *
 * \brief This class is used to store multiple
 *        unique(by name) SettingGroup
 *
 */

#include <QObject>
#include <vector>
#include "setting.h"
//#include "SettingGroupDeclaration.h"

namespace Settings
{
    class SettingGroup  :   public QObject
    {
            Q_OBJECT

        public:
            SettingGroup();

            /**
             * \brief Constructor
             * \param name, name of this SettingGroup group
             */
            SettingGroup(const QString &name);

            /**
             * \brief Copy constructor
             * \param other, The other object from which will be copied
             */
            SettingGroup(const SettingGroup &other);
            ~SettingGroup();

            /**
             * \brief setName
             * \param name, name of this SettingGroup group
             */
            void setName(const QString &name);

            /**
             * \brief getName
             * \return returns the name of this SettingGroup group
             */
            const QString &getName() const;

            /**
             * \brief operator=
             * \param other, The other object from which will be copied
             * \return returns a reference to this
             */
            const SettingGroup &operator=(const SettingGroup &other);

            /**
             * \brief operator[]
             * \param name, the name of the setting you try to access
             * \return returns the setting with the name
             *         returns a dummy setting if the index is out of range
             */
            Setting &operator[](const QString &name);

            /**
             * \brief operator[]
             * \param index, index of the setting
             * \return returns the setting at that index
             *         returns a dummy setting if the index is out of range
             */
            Setting &operator[](size_t index);

            /**
             * \brief add
             * \details adds the setting to the list if no setting with the same
             *          name already exists in the list.
             * \param setting, the setting which will be added
             * \return true, if the setting was added successfully
             *         false, if the setting could not been added
             */
            bool add(const Setting &setting);

            /**
             * \brief add
             * \param name, name of the setting
             * \param value, value of the setting
             * \see add()
             */
            bool add(const QString &name, const QVariant value);
            /**
             * \brief add
             * \param setting, pair of name and value for the setting
             * \see add()
             */
            bool add(const std::pair<QString,QVariant> &setting);

            /**
             * \brief remove
             * \details removes a setting with the same name as the setting parameter
             * \param setting, the setting which shuld be removed
             * \return true, if the setting was removed successfully
             *         false, if the setting was not in the list
             */
            bool remove(const Setting &setting);

            /**
             * \brief remove
             * \param name, the name of the setting which shuld be removed
             * \see remove()
             */
            bool remove(const QString &name);

            /**
             * \brief remove
             * \param index, the index of the setting which shuld be removed
             * \see remove()
             */
            bool remove(size_t index);

            /**
             * \brief removes all SettingGroup from the list
             */
            void clear();

            /**
             * \brief exists
             * \param name, the name of the setting which will be searched for
             * \return true, if the setting with the name exists in the list
             *         false, if the setting with the name does not exist in the list
             */
            bool exists(const QString &name) const;

            /**
             * \brief getIndex
             * \param name, the name of the setting which will be searched for
             * \return returns the index of setting with the name list
             *         returns SettingGroup::npos, if no such setting exists in the list
             */
            size_t getIndex(const QString &name) const;

            /**
             * \brief getSize
             * \return returns the amount of SettingGroup in the list
             */
            size_t getSize() const;

            /**
             * \brief toString
             * \param tabCount, amount of tabs which will be at the left side of every
             *                  new line.
             *                  One tab = 4 spaced = "    "
             * \return returns a string with all informations of this SettingGroup
             */
            QString toString(int tabCount = 0) const;

            /**
             * \brief debug operator
             *        Prints the setting to the QT-debug console in the same form as this->toString()
             * \see toString()
             *        Usage:
             *        SettingGroup mySettingGroup();
             *        qDebug() << mySettingGroup;
             */
            friend QDebug operator<<(QDebug debug, const SettingGroup &SettingGroup);


            static const size_t npos = -1; //!< The no position index of an array
        signals:
            /**
             * \brief settingValueChanged
             * \details Will be emitted if the setting value has been changed
             */
            void settingValueChanged(const Setting &setting);

            /**
             * \brief settingNameChanged
             * \details Will be emitted if the setting name has been changed
             */
            void settingNameChanged(const Setting &setting);

            /**
             * \brief settingAdded
             * \details Will be emitted if a setting was added to the list
             */
            void settingAdded(const Setting &setting);

            /**
             * \brief settingRemoved
             * \details Will be emitted a setting got removed from the list
             */
            void settingRemoved();

            /**
             * \brief SettingGroupCleared
             * \details Will be emitted if the SettingGroup list was cleared
             */
            void SettingGroupCleared();
        public slots:

        private slots:
            void onSettingValueChanged(const QVariant &value);
            void onSettingNameChanged(const QString &parameterName);
            void onSettingDestroyed(QObject *obj);

        protected:

        private:
            void connectSignals(Setting *setting);
            void disconnectSignals(Setting *setting);

            QString m_name;
            std::vector<Setting*> m_SettingGroup;

    };
}
Q_DECLARE_METATYPE(Settings::SettingGroup);
