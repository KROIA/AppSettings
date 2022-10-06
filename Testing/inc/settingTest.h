#pragma once

#include "subTest.h"
#include "setting.h"



class SettingTest : public SubTest
{
    Q_OBJECT
    public:
        SettingTest();
        static void initMain();


    private slots:
        void initTestCase();
        void cleanupTestCase();

        void createSetting();

    public slots:
        void onSettingValueChanged(const QVariant &value);
        void onSettingNameChanged(const QString &name);
    private:
        Settings::Setting *m_setting;

        static const QString m_settingName;
};
