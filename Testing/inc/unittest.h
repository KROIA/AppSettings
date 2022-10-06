#pragma once

#include <QTest>
#include <QSignalSpy>
#include "settings.h"
#include "settingTest.h"


class SettingsUnitTest : public QObject
{
    Q_OBJECT
    public:
        SettingsUnitTest();
        static void initMain();

    private slots:
        void initTestCase();
        void cleanupTestCase();

        void createSettings();
        void editSettings();
        void removeSettings();
        void clearSettings();

    public slots:
        void onSettingValueChanged(const Settings::Setting &setting);
        void onSettingNameChanged(const Settings::Setting &setting);
    private:
        Settings::Settings *m_settings;



        static const QString m_settingsName;
};
