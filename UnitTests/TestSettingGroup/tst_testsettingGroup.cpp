#include <QtTest>
#include "settingGroup.h"
// add necessary includes here

using namespace Settings;

class TestSettingGroup : public QObject
{
        Q_OBJECT

    public:
        TestSettingGroup();
        ~TestSettingGroup();

    private slots:
        void initTestCase();
        void cleanupTestCase();

        void createSettings();
        void editSettings();
        void removeSettings();
        void clearSettings();

    public slots:
        void onSettingValueChanged(Setting *setting);
        void onSettingNameChanged(Setting *setting);

    private:
        SettingGroup *m_settings;



        static const QString m_settingsName;
};

const QString TestSettingGroup::m_settingsName = "MySettings";

TestSettingGroup::TestSettingGroup()
{

}

TestSettingGroup::~TestSettingGroup()
{

}

void TestSettingGroup::initTestCase()
{
    qDebug("initTestCase");
   // TestSettingGroup TestSettingGroup;

   // QTest::qExec(&TestSettingGroup);


    m_settings = new SettingGroup(m_settingsName);
    connect(m_settings,&SettingGroup::settingNameChanged,this,&TestSettingGroup::onSettingNameChanged);
    connect(m_settings,&SettingGroup::settingValueChanged,this,&TestSettingGroup::onSettingValueChanged);
    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSettingsCount() == 0);
}
void TestSettingGroup::cleanupTestCase()
{
    delete m_settings;
    m_settings = nullptr;
    qDebug("cleanupTestCase");
}


void TestSettingGroup::createSettings()
{
    QSignalSpy settingsAddedSpy(m_settings, &SettingGroup::settingAdded);
    m_settings->add("FirstSetting",123);
    QCOMPARE(settingsAddedSpy.count(), 1);
    m_settings->add("SecondSetting","text");
    QCOMPARE(settingsAddedSpy.count(), 2);
    m_settings->add("ThirtSetting",1.58);
    QCOMPARE(settingsAddedSpy.count(), 3);
    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSettingsCount() == 3);

    QCOMPARE((*m_settings).getSetting("FirstSetting")->getValue().toInt(),123);
    QCOMPARE((*m_settings).getSetting("SecondSetting")->getValue().toString(),"text");
    QCOMPARE((*m_settings).getSetting("ThirtSetting")->getValue().toDouble(),1.58);

    QCOMPARE((*m_settings).getSetting(0)->getValue().toInt(),123);
    QCOMPARE((*m_settings).getSetting(1)->getValue().toString(),"text");
    QCOMPARE((*m_settings).getSetting(2)->getValue().toDouble(),1.58);

    QVERIFY((*m_settings).getSetting(3) == nullptr); // Must QWarning
    QVERIFY((*m_settings).getSetting("noone") == nullptr); // Must QWarning
    QVERIFY((*m_settings).getSetting(-5) == nullptr); // Must QWarning

}
void TestSettingGroup::editSettings()
{
    QSignalSpy settingsChangedSpy(m_settings, &SettingGroup::settingValueChanged);
    (*m_settings).getSetting("FirstSetting")->setValue(321);
    QCOMPARE(settingsChangedSpy.count(), 1);
    (*m_settings).getSetting("SecondSetting")->setValue(222);
    QCOMPARE(settingsChangedSpy.count(), 2);
    (*m_settings).getSetting("ThirtSetting")->setValue("newText");
    QCOMPARE(settingsChangedSpy.count(), 3);

    QCOMPARE((*m_settings).getSetting("FirstSetting")->getValue().toInt(),321);
    QCOMPARE((*m_settings).getSetting("SecondSetting")->getValue().toInt(),222);
    QCOMPARE((*m_settings).getSetting("ThirtSetting")->getValue().toString(),"newText");

    QCOMPARE((*m_settings).getSetting(0)->getValue().toInt(),321);
    QCOMPARE((*m_settings).getSetting(1)->getValue().toInt(),222);
    QCOMPARE((*m_settings).getSetting(2)->getValue().toString(),"newText");

}
void TestSettingGroup::removeSettings()
{
    QSignalSpy settingsRemovedSpy(m_settings, &SettingGroup::settingRemoved);
    QVERIFY(m_settings->removeSetting("SecondSetting") != nullptr);
    QCOMPARE(settingsRemovedSpy.count(), 1);
    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSettingsCount() == 2);

    QCOMPARE((*m_settings).getSetting("FirstSetting")->getValue().toInt(),321);
    QVERIFY((*m_settings).getSetting("SecondSetting") == nullptr);          // Must QWarning
    QCOMPARE((*m_settings).getSetting("ThirtSetting")->getValue().toString(),"newText");

    QCOMPARE((*m_settings).getSetting(0)->getValue().toInt(),321);
    QCOMPARE((*m_settings).getSetting(1)->getValue().toString(),"newText");

}
void TestSettingGroup::clearSettings()
{
    m_settings->clear();

    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSettingsCount() == 0);

    QVERIFY((*m_settings).getSetting("FirstSetting")  == nullptr); // Must QWarning
    QVERIFY((*m_settings).getSetting("SecondSetting") == nullptr); // Must QWarning
    QVERIFY((*m_settings).getSetting("ThirtSetting")  == nullptr); // Must QWarning
}


void TestSettingGroup::onSettingValueChanged(Setting *setting)
{
    qDebug() << "onSettingValueChanged: "<<*setting;
}
void TestSettingGroup::onSettingNameChanged(Setting *setting)
{
    qDebug() << "onSettingNameChanged: "<<*setting;
}

QTEST_APPLESS_MAIN(TestSettingGroup)

#include "tst_testsettingGroup.moc"
