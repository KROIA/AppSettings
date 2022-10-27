#pragma once


#include <QObject>
#include <vector>
//#include "Setting.h"
#include "SettingsDeclaration.h"

namespace Settings
{

    /**
     * \author  Alex Krieg
     * \version 00.00.01
     * \date    27.10.2022
     *
     * \brief This class is used to store multiple
     *        unique(by name) Settings
     *
     */
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
            //SettingGroup(const SettingGroup &other, SettingGroup *parent = nullptr);
            virtual ~SettingGroup();

            SettingGroup *getParent() const;

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

            bool isEmpty() const;

            /**
             * \brief operator=
             * \param other, The other object from which will be copied
             * \return returns a reference to this
             */
            //const SettingGroup &operator=(const SettingGroup &other);

            /**
             * \brief operator[]
             * \param name, the name of the setting you try to access
             * \return returns the setting with the name
             *         returns a dummy setting if the index is out of range
             */
            //Setting &operator[](const QString &name);

            /**
             * \brief operator[]
             * \param index, index of the setting
             * \return returns the setting at that index
             *         returns a dummy setting if the index is out of range
             */
            //Setting &operator[](size_t index);

            /**
             * \brief add
             * \details adds the setting to the list if no setting with the same
             *          name already exists in the list.
             * \param setting, the setting which will be added
             * \return true, if the setting was added successfully
             *         false, if the setting could not been added
             */
            bool add(Setting *setting);

            /**
             * \brief add
             * \param name, name of the setting
             * \param value, value of the setting
             * \return pointer to the added setting. nullptr if setting could not been created.
             */
            Setting *add(const QString &name, const QVariant &value);

            /**
             * \brief add
             * \details adds the setting to the list if no setting with the same
             *          name already exists in the list.
             * \param setting, the setting which will be added
             * \return true, if the setting was added successfully
             *         false, if the setting could not been added
             */
            bool add(SettingGroup *group);

            /**
             * \brief add
             * \param setting, pair of name and value for the setting
             * \see add()
             */
            //bool add(const std::pair<QString,QVariant> &setting);

            /**
             * \brief remove
             * \details removes a setting with the same name as the setting parameter.
             *          It will not be deleted on removal.
             * \param setting, the setting which shuld be removed
             * \return true, if the setting was removed successfully
             *         false, if the setting was not in the list
             */
            bool removeSetting(Setting *setting);

            /**
             * \brief removeSetting
             * \param name, the name of the setting which shuld be removed
             * \see remove()
             * \return the removed setting. It will not be deleted on removal.
             *         nullptr if the setting was not found.
             */
            Setting *removeSetting(const QString &name);

            /**
             * \brief removeSetting
             * \param index, the index of the setting which shuld be removed
             * \see remove()
             * \return the removed setting. It will not be deleted on removal.
             *         nullptr if the setting was not found.
             */
            Setting *removeSetting(size_t index);

            /**
             * \brief removeGroup
             * \details removes a setting group with the same name as the group parameter.
             *          It will not be deleted on removal.
             * \param group, the setting group which shuld be removed
             * \return the removed setting group. It will not be deleted on removal.
             *         nullptr if the setting was not found.
             */
            SettingGroup *removeGroup(SettingGroup *group);

            /**
             * \brief removeGroup
             * \param name, the name of the setting group which shuld be removed
             * \see remove()
             * \return the removed setting group. It will not be deleted on removal.
             *         nullptr if the setting was not found.
             */
            SettingGroup *removeGroup(const QString &name);

            /**
             * \brief removeGroup
             * \param index, the index of the setting group which shuld be removed
             * \see remove()
             * \return the removed setting group. It will not be deleted on removal.
             *         nullptr if the setting was not found.
             */
            SettingGroup *removeGroup(size_t index);

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
            bool settingExists(const QString &name) const;
            bool childGroupExists(const QString &name) const;

            /**
             * \brief getIndex
             * \param name, the name of the setting which will be searched for
             * \return returns the index of setting with the name list
             *         returns SettingGroup::npos, if no such setting exists in the list
             */
            size_t getSettingIndex(const QString &name) const;
            size_t getChildGroupIndex(const QString &name) const;

            Setting *getSetting(size_t index) const;
            Setting *getSetting(const QString &name) const;
            SettingGroup *getChildGroup(size_t index) const;
            SettingGroup *getChildGroup(const QString &name) const;

            template<typename T>
            T *getSetting(size_t index) const;

            template<typename T>
            T *getSetting(const QString &name) const;

            template<typename T>
            T *getChildGroup(size_t index) const;

            template<typename T>
            T *getChildGroup(const QString &name) const;

            /**
             * \brief getSettingsCount
             * \return returns the amount of settings in the list
             */
            size_t getSettingsCount() const;

            /**
             * \brief getChildGroupCount
             * \return returns the amount of setting groups in the list
             */
            size_t getChildGroupCount() const;

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
            void settingValueChanged(Setting *setting);

            /**
             * \brief settingNameChanged
             * \details Will be emitted if the setting name has been changed
             */
            void settingNameChanged(Setting *setting);

            /**
             * \brief settingAdded
             * \details Will be emitted if a setting was added to the list
             */
            void settingAdded(Setting *setting);

            /**
             * \brief childGroupAdded
             * \details Will be emitted if a setting group was added to the list
             */
            void childGroupAdded(SettingGroup *group);

            /**
             * \brief settingRemoved
             * \param setting which got removed. It was not deleted.
             * \details Will be emitted if a setting got removed from the list
             */
            void settingRemoved(Setting *setting);

            /**
             * \brief childGroupRemoved
             * \param group which got removed. It was not deleted.
             * \details Will be emitted if a child SettingsList got removed
             */
            void childGroupRemoved(SettingGroup *group);

            /**
             * \brief settingDeleted
             * \details Will be emitted if a setting got deleted
             */
            void settingDeleted();

            /**
             * \brief childGroupDeleted
             * \details Will be emitted if a child SettingsList got deleted
             */
            void childGroupDeleted();

            /**
             * \brief SettingGroupCleared
             * \details Will be emitted if the SettingGroup list was cleared
             */
            void cleared();

            /**
             * \brief nameChanged
             * \details Will be emitted if the SettingGroup name has been changed
             */
            void nameChanged(const QString &name);

        public slots:

        private slots:
            void onSettingValueChanged(const QVariant &value);
            void onSettingNameChanged(const QString &parameterName);
            void onSettingDestroyed(QObject *obj);

            //void onChildGroupNameChanged(const QString &groupName);

        protected:         
            void connectSignals(Setting *setting);
            void disconnectSignals(Setting *setting);
            void connectSignals(SettingGroup *settings);
            void disconnectSignals(SettingGroup *settings);
            void connectSignals();
            void disconnectSignals();

            virtual Setting *createSettingInstance();

            QString m_name;
            SettingGroup *m_parent;
            std::vector<Setting*> m_settings;
            std::vector<SettingGroup*> m_settingGroups;

            bool m_isEmpty; // true if no setting got added.

    };

    template<typename T>
    T *SettingGroup::getSetting(size_t index) const
    {
        return dynamic_cast<T*>(getSetting(index));
    }

    template<typename T>
    T *SettingGroup::getSetting(const QString &name) const
    {
        return dynamic_cast<T*>(getSetting(name));
    }

    template<typename T>
    T *SettingGroup::getChildGroup(size_t index) const
    {
        return dynamic_cast<T*>(getChildGroup(index));
    }

    template<typename T>
    T *SettingGroup::getChildGroup(const QString &name) const
    {
        return dynamic_cast<T*>(getChildGroup(name));
    }
}
Q_DECLARE_METATYPE(Settings::SettingGroup);
