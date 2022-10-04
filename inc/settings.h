#pragma once
#include <QObject>
#include <vector>
#include "setting.h"


namespace Settings
{
    using std::vector;
    class Settings  :   public QObject
    {
            Q_OBJECT

        public:
            Settings();
            Settings(const QString &name);
            Settings(const Settings &other);
            ~Settings();

            void setName(const QString &name);
            const QString &getName() const;

            const Settings &operator=(const Settings &other);

            Setting &operator[](const QString &name);
            Setting &operator[](size_t index);
            bool add(const Setting &setting);
            bool add(const QString &name, const QVariant value);
            bool add(const std::pair<QString,QVariant> &setting);
            bool remove(const Setting &setting);
            bool remove(const QString &name);
            bool remove(size_t index);
            void clear();

            inline bool exists(const QString &name) const;
            size_t getIndex(const QString &name) const;
            size_t getSize() const;

            QString toString(int tabCount = 0) const;
            friend QDebug operator<<(QDebug debug, const Settings &settings);

        signals:
            void settingValueChanged(const Setting &setting);
            void settingNameChanged(const Setting &setting);
            void settingAdded();
            void settingRemoved();
            void settingsCleared();
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
            vector<Setting*> m_settings;

    };
}
Q_DECLARE_METATYPE(Settings::Settings);
