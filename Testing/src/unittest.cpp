#include "unittest.h"


const QString SettingsUnitTest::m_settingsName = "MySettings";

SettingsUnitTest::SettingsUnitTest()
{
    m_settings = nullptr;
}

void SettingsUnitTest::initMain()
{
    qDebug("initMain");


}


void SettingsUnitTest::initTestCase()
{
    qDebug("initTestCase");
    //QTest::qExec(obj, argc, argv);
    /*SettingTest settingTest;
    settingTest.initTestCase();
    settingTest.createSetting();*/

    m_settings = new Settings::Settings(m_settingsName);
    connect(m_settings,&Settings::Settings::settingNameChanged,this,&SettingsUnitTest::onSettingNameChanged);
    connect(m_settings,&Settings::Settings::settingValueChanged,this,&SettingsUnitTest::onSettingValueChanged);
    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSize() == 0);
}
void SettingsUnitTest::cleanupTestCase()
{
    delete m_settings;
    m_settings = nullptr;
    qDebug("cleanupTestCase");
}


void SettingsUnitTest::createSettings()
{
    QSignalSpy settingsAddedSpy(m_settings, &Settings::Settings::settingAdded);
    m_settings->add("FirstSetting",123);
    QCOMPARE(settingsAddedSpy.count(), 1);
    m_settings->add("SecondSetting","text");
    QCOMPARE(settingsAddedSpy.count(), 2);
    m_settings->add("ThirtSetting",1.58);
    QCOMPARE(settingsAddedSpy.count(), 3);
    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSize() == 3);

    QCOMPARE((*m_settings)["FirstSetting"].getValue().toInt(),123);
    QCOMPARE((*m_settings)["SecondSetting"].getValue().toString(),"text");
    QCOMPARE((*m_settings)["ThirtSetting"].getValue().toDouble(),1.58);

    QCOMPARE((*m_settings)[0].getValue().toInt(),123);
    QCOMPARE((*m_settings)[1].getValue().toString(),"text");
    QCOMPARE((*m_settings)[2].getValue().toDouble(),1.58);

    QCOMPARE((*m_settings)[3].getValue().toDouble(),0); // Must QWarning
    QCOMPARE((*m_settings)["noone"].getValue().toInt(),0); // Must QWarning
    QCOMPARE((*m_settings)[-5].getValue().toInt(),0); // Must QWarning

}
void SettingsUnitTest::editSettings()
{
    QSignalSpy settingsChangedSpy(m_settings, &Settings::Settings::settingValueChanged);
    (*m_settings)["FirstSetting"] = 321;
    QCOMPARE(settingsChangedSpy.count(), 1);
    (*m_settings)["SecondSetting"] = 222;
    QCOMPARE(settingsChangedSpy.count(), 2);
    (*m_settings)["ThirtSetting"] = "newText";
    QCOMPARE(settingsChangedSpy.count(), 3);

    QCOMPARE((*m_settings)["FirstSetting"].getValue().toInt(),321);
    QCOMPARE((*m_settings)["SecondSetting"].getValue().toInt(),222);
    QCOMPARE((*m_settings)["ThirtSetting"].getValue().toString(),"newText");

    QCOMPARE((*m_settings)[0].getValue().toInt(),321);
    QCOMPARE((*m_settings)[1].getValue().toInt(),222);
    QCOMPARE((*m_settings)[2].getValue().toString(),"newText");

}
void SettingsUnitTest::removeSettings()
{
    QSignalSpy settingsRemovedSpy(m_settings, &Settings::Settings::settingRemoved);
    QVERIFY(m_settings->remove("SecondSetting"));
    QCOMPARE(settingsRemovedSpy.count(), 1);
    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSize() == 2);

    QCOMPARE((*m_settings)["FirstSetting"].getValue().toInt(),321);
    QCOMPARE((*m_settings)["SecondSetting"].getValue().toInt(),0);          // Must QWarning
    QCOMPARE((*m_settings)["ThirtSetting"].getValue().toString(),"newText");

    QCOMPARE((*m_settings)[0].getValue().toInt(),321);
    QCOMPARE((*m_settings)[1].getValue().toString(),"newText");

}
void SettingsUnitTest::clearSettings()
{
    m_settings->clear();

    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSize() == 0);

    QCOMPARE((*m_settings)["FirstSetting"].getValue().toInt(),0);    // Must QWarning
    QCOMPARE((*m_settings)["SecondSetting"].getValue().toInt(),0);   // Must QWarning
    QCOMPARE((*m_settings)["ThirtSetting"].getValue().toString(),"");// Must QWarning
}


void SettingsUnitTest::onSettingValueChanged(const Settings::Setting &setting)
{
    qDebug() << "onSettingValueChanged: "<<setting;
}
void SettingsUnitTest::onSettingNameChanged(const Settings::Setting &setting)
{
    qDebug() << "onSettingNameChanged: "<<setting;
}
