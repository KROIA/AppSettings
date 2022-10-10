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
        void onSettingValueChanged(const Setting &setting);
        void onSettingNameChanged(const Setting &setting);

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
    QVERIFY(m_settings->getSize() == 0);
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
void TestSettingGroup::editSettings()
{
    QSignalSpy settingsChangedSpy(m_settings, &SettingGroup::settingValueChanged);
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
void TestSettingGroup::removeSettings()
{
    QSignalSpy settingsRemovedSpy(m_settings, &SettingGroup::settingRemoved);
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
void TestSettingGroup::clearSettings()
{
    m_settings->clear();

    QCOMPARE(m_settings->getName(),m_settingsName);
    QVERIFY(m_settings->getSize() == 0);

    QCOMPARE((*m_settings)["FirstSetting"].getValue().toInt(),0);    // Must QWarning
    QCOMPARE((*m_settings)["SecondSetting"].getValue().toInt(),0);   // Must QWarning
    QCOMPARE((*m_settings)["ThirtSetting"].getValue().toString(),"");// Must QWarning
}


void TestSettingGroup::onSettingValueChanged(const Setting &setting)
{
    qDebug() << "onSettingValueChanged: "<<setting;
}
void TestSettingGroup::onSettingNameChanged(const Setting &setting)
{
    qDebug() << "onSettingNameChanged: "<<setting;
}

QTEST_APPLESS_MAIN(TestSettingGroup)

#include "tst_testsettingGroup.moc"
