#include <QtTest>
#include "settings.h"
// add necessary includes here

class TestSettings : public QObject
{
        Q_OBJECT

    public:
        TestSettings();
        ~TestSettings();

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

const QString TestSettings::m_settingsName = "MySettings";

TestSettings::TestSettings()
{

}

TestSettings::~TestSettings()
{

}

void TestSettings::initTestCase()
{
    qDebug("initTestCase");
   // TestSettings TestSettings;

   // QTest::qExec(&TestSettings);


    m_settings = new Settings::Settings(m_settingsName);
    connect(m_settings,&Settings::Settings::settingNameChanged,this,&TestSettings::onSettingNameChanged);
    connect(m_settings,&Settings::Settings::settingValueChanged,this,&TestSettings::onSettingValueChanged);
    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSize() == 0);
}
void TestSettings::cleanupTestCase()
{
    delete m_settings;
    m_settings = nullptr;
    qDebug("cleanupTestCase");
}


void TestSettings::createSettings()
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
void TestSettings::editSettings()
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
void TestSettings::removeSettings()
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
void TestSettings::clearSettings()
{
    m_settings->clear();

    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSize() == 0);

    QCOMPARE((*m_settings)["FirstSetting"].getValue().toInt(),0);    // Must QWarning
    QCOMPARE((*m_settings)["SecondSetting"].getValue().toInt(),0);   // Must QWarning
    QCOMPARE((*m_settings)["ThirtSetting"].getValue().toString(),"");// Must QWarning
}


void TestSettings::onSettingValueChanged(const Settings::Setting &setting)
{
    qDebug() << "onSettingValueChanged: "<<setting;
}
void TestSettings::onSettingNameChanged(const Settings::Setting &setting)
{
    qDebug() << "onSettingNameChanged: "<<setting;
}

QTEST_APPLESS_MAIN(TestSettings)

#include "tst_testsettings.moc"
