#include "settingTest.h"


const QString SettingTest::m_settingName = "MySetting";

SettingTest::SettingTest()
{
    m_setting = nullptr;
}

void SettingTest::initMain()
{
    qDebug("initMain");


}


void SettingTest::initTestCase()
{
    qDebug("SettingTest::initTestCase");

    m_setting = new Settings::Setting(m_settingName,"");
    connect(m_setting,&Settings::Setting::nameChanged,this,&SettingTest::onSettingNameChanged);
    connect(m_setting,&Settings::Setting::valueChanged,this,&SettingTest::onSettingValueChanged);
    QCOMPARE(m_setting->getName(),m_settingName);
}
void SettingTest::cleanupTestCase()
{
    delete m_setting;
    m_setting = nullptr;
    qDebug("cleanupTestCase");
}


void SettingTest::createSetting()
{
    QVERIFY(1);

}



void SettingTest::onSettingValueChanged(const QVariant &value)
{
    qDebug() << "onSettingValueChanged: "<<value;
}
void SettingTest::onSettingNameChanged(const QString &name)
{
    qDebug() << "onSettingNameChanged: "<<name;
}
