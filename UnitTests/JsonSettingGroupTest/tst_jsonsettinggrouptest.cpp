#include <QtTest>
#include "JsonSetting.h"
#include "JsonSettingGroup.h"
#include <QJsonObject>

// add necessary includes here
using namespace Settings;

class JsonSettingGroupTest : public QObject
{
        Q_OBJECT

    public:
        JsonSettingGroupTest();
        ~JsonSettingGroupTest();

    private slots:
        void saveLoad();
        void saveLoadNested();
        void toAndFromFile();

};

JsonSettingGroupTest::JsonSettingGroupTest()
{

}

JsonSettingGroupTest::~JsonSettingGroupTest()
{

}

void JsonSettingGroupTest::saveLoad()
{
    JsonSettingGroup g1("group1");
    JsonSetting *s1 = new JsonSetting("setting1","value1");
    JsonSetting *s2 = new JsonSetting("setting2","value2");
    JsonSetting *s3 = new JsonSetting("setting3","value3");
    JsonSetting *s4 = new JsonSetting("setting4","value4");

    g1.add(s1);
    g1.add(s2);
    g1.add(s3);
    g1.add(s4);

    QVERIFY(g1.getSettingsCount() == 4);
    QJsonObject data = g1.save();


    JsonSettingGroup g2("group2");
    QVERIFY(g2.read(data));
    QCOMPARE(g2.getName(), g1.getName());
    QVERIFY(g2.getSettingsCount() == 4);


    Setting *s21 = g2.getSetting("setting1");
    Setting *s22 = g2.getSetting("setting2");
    JsonSetting *s23 = g2.getSetting<JsonSetting>("setting3");
    JsonSetting *s24 = g2.getSetting<JsonSetting>("setting4");

    QVERIFY(s21 != nullptr);
    QVERIFY(s22 != nullptr);
    QVERIFY(s23 != nullptr);
    QVERIFY(s24 != nullptr);

    if(s1)
    {
        QVERIFY(dynamic_cast<JsonSetting*>(s21) != nullptr);
        QCOMPARE(s21->getValue(), s1->getValue());
    }

    if(s22)
    QCOMPARE(s22->getValue(), s2->getValue());
    if(s23)
    QCOMPARE(s23->getValue(), s3->getValue());
    if(s24)
    QCOMPARE(s24->getValue(), s4->getValue());


}

void JsonSettingGroupTest::saveLoadNested()
{
    JsonSettingGroup *g1 = new JsonSettingGroup("group1");
    JsonSetting *s1_1 = new JsonSetting("setting1_1","value1_1");
    JsonSetting *s1_2 = new JsonSetting("setting1_2","value1_2");
    JsonSetting *s1_3 = new JsonSetting("setting1_3","value1_3");
    JsonSetting *s1_4 = new JsonSetting("setting1_4","value1_4");

    g1->add(s1_1);
    g1->add(s1_2);
    g1->add(s1_3);
    g1->add(s1_4);

    JsonSettingGroup *g2 = new JsonSettingGroup("group2");
    JsonSetting *s2_1 = new JsonSetting("setting2_1","value2_1");
    JsonSetting *s2_2 = new JsonSetting("setting2_2","value2_2");
    JsonSetting *s2_3 = new JsonSetting("setting2_3","value2_3");
    JsonSetting *s2_4 = new JsonSetting("setting2_4","value2_4");

    g2->add(s2_1);
    g2->add(s2_2);
    g2->add(s2_3);
    g2->add(s2_4);

    g1->add(g2);
    QVERIFY(g1->getSettingsCount() == 4);
    QVERIFY(g1->getChildGroupCount() == 1);

    QJsonObject data = g1->save();

    JsonSettingGroup g3("group3");
    QVERIFY(g3.read(data));
    QCOMPARE(g3.getName(), g1->getName());
    QVERIFY(g3.getSettingsCount() == 4);
    QVERIFY(g3.getChildGroupCount() == 1);

    Setting *s3_1 = g3.getSetting("setting1_1");
    Setting *s3_2 = g3.getSetting("setting1_2");
    JsonSetting *s3_3 = g3.getSetting<JsonSetting>("setting1_3");
    JsonSetting *s3_4 = g3.getSetting<JsonSetting>("setting1_4");

    QVERIFY(s3_1 != nullptr);
    QVERIFY(s3_2 != nullptr);
    QVERIFY(s3_3 != nullptr);
    QVERIFY(s3_4 != nullptr);

    if(s3_1)
    {
        QVERIFY(dynamic_cast<JsonSetting*>(s3_1) != nullptr);
        QCOMPARE(s3_1->getValue(), s1_1->getValue());
    }

    if(s3_2)
    QCOMPARE(s3_2->getValue(), s1_2->getValue());
    if(s3_3)
    QCOMPARE(s3_3->getValue(), s1_3->getValue());
    if(s3_4)
    QCOMPARE(s3_4->getValue(), s1_4->getValue());


    JsonSettingGroup *g4 = g3.getChildGroup<JsonSettingGroup>("group2");
    QVERIFY(g4 != nullptr);
    if(g4)
    {
        Setting *s4_1 = g4->getSetting("setting2_1");
        Setting *s4_2 = g4->getSetting("setting2_2");
        JsonSetting *s4_3 = g4->getSetting<JsonSetting>("setting2_3");
        JsonSetting *s4_4 = g4->getSetting<JsonSetting>("setting2_4");

        QVERIFY(s4_1 != nullptr);
        QVERIFY(s4_2 != nullptr);
        QVERIFY(s4_3 != nullptr);
        QVERIFY(s4_4 != nullptr);

        if(s4_1)
        {
            QVERIFY(dynamic_cast<JsonSetting*>(s4_1) != nullptr);
            QCOMPARE(s4_1->getValue(), s2_1->getValue());
        }

        if(s4_2)
        QCOMPARE(s4_2->getValue(), s2_2->getValue());
        if(s4_3)
        QCOMPARE(s4_3->getValue(), s2_3->getValue());
        if(s4_4)
        QCOMPARE(s4_4->getValue(), s2_4->getValue());
    }
    delete g1;
}

void JsonSettingGroupTest::toAndFromFile()
{
    JsonSettingGroup *g1 = new JsonSettingGroup("group1");
    JsonSetting *s1_1 = new JsonSetting("setting1_1","value1_1");
    JsonSetting *s1_2 = new JsonSetting("setting1_2","value1_2");
    JsonSetting *s1_3 = new JsonSetting("setting1_3","value1_3");
    JsonSetting *s1_4 = new JsonSetting("setting1_4","value1_4");

    g1->add(s1_1);
    g1->add(s1_2);
    g1->add(s1_3);
    g1->add(s1_4);

    JsonSettingGroup *g2 = new JsonSettingGroup("group2");
    JsonSetting *s2_1 = new JsonSetting("setting2_1","value2_1");
    JsonSetting *s2_2 = new JsonSetting("setting2_2","value2_2");
    JsonSetting *s2_3 = new JsonSetting("setting2_3","value2_3");
    JsonSetting *s2_4 = new JsonSetting("setting2_4","value2_4");

    g2->add(s2_1);
    g2->add(s2_2);
    g2->add(s2_3);
    g2->add(s2_4);

    g1->add(g2);
    QVERIFY(g1->getSettingsCount() == 4);
    QVERIFY(g1->getChildGroupCount() == 1);

    QString file = "testSettings.json";
    QVERIFY(g1->saveToFile(file));

    JsonSettingGroup g3("group3");
    QVERIFY(g3.readFromFile(file));
    QCOMPARE(g3.getName(), g1->getName());
    QVERIFY(g3.getSettingsCount() == 4);
    QVERIFY(g3.getChildGroupCount() == 1);

    Setting *s3_1 = g3.getSetting("setting1_1");
    Setting *s3_2 = g3.getSetting("setting1_2");
    JsonSetting *s3_3 = g3.getSetting<JsonSetting>("setting1_3");
    JsonSetting *s3_4 = g3.getSetting<JsonSetting>("setting1_4");

    QVERIFY(s3_1 != nullptr);
    QVERIFY(s3_2 != nullptr);
    QVERIFY(s3_3 != nullptr);
    QVERIFY(s3_4 != nullptr);

    if(s3_1)
    {
        QVERIFY(dynamic_cast<JsonSetting*>(s3_1) != nullptr);
        QCOMPARE(s3_1->getValue(), s1_1->getValue());
    }

    if(s3_2)
    QCOMPARE(s3_2->getValue(), s1_2->getValue());
    if(s3_3)
    QCOMPARE(s3_3->getValue(), s1_3->getValue());
    if(s3_4)
    QCOMPARE(s3_4->getValue(), s1_4->getValue());


    JsonSettingGroup *g4 = g3.getChildGroup<JsonSettingGroup>("group2");
    QVERIFY(g4 != nullptr);
    if(g4)
    {
        Setting *s4_1 = g4->getSetting("setting2_1");
        Setting *s4_2 = g4->getSetting("setting2_2");
        JsonSetting *s4_3 = g4->getSetting<JsonSetting>("setting2_3");
        JsonSetting *s4_4 = g4->getSetting<JsonSetting>("setting2_4");

        QVERIFY(s4_1 != nullptr);
        QVERIFY(s4_2 != nullptr);
        QVERIFY(s4_3 != nullptr);
        QVERIFY(s4_4 != nullptr);

        if(s4_1)
        {
            QVERIFY(dynamic_cast<JsonSetting*>(s4_1) != nullptr);
            QCOMPARE(s4_1->getValue(), s2_1->getValue());
        }

        if(s4_2)
        QCOMPARE(s4_2->getValue(), s2_2->getValue());
        if(s4_3)
        QCOMPARE(s4_3->getValue(), s2_3->getValue());
        if(s4_4)
        QCOMPARE(s4_4->getValue(), s2_4->getValue());
    }
    delete g1;
}

QTEST_APPLESS_MAIN(JsonSettingGroupTest)

#include "tst_jsonsettinggrouptest.moc"
