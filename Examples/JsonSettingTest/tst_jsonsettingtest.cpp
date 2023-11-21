#include <QtTest>
#include "JsonSetting.h"
#include <QJsonObject>

// add necessary includes here
using namespace Settings;

class JsonSettingTest : public QObject
{
        Q_OBJECT

    public:
        JsonSettingTest();
        ~JsonSettingTest();

    private slots:
        void saveLoad();

};

JsonSettingTest::JsonSettingTest()
{

}

JsonSettingTest::~JsonSettingTest()
{

}

void JsonSettingTest::saveLoad()
{
    JsonSetting setting1("setting","value");
    QJsonObject data = setting1.save();

    JsonSetting setting2;
    QVERIFY(setting2.read(data));
    QCOMPARE(setting2.getName() , setting1.getName());
    QCOMPARE(setting2.getValue() , setting1.getValue());
}

QTEST_APPLESS_MAIN(JsonSettingTest)

#include "tst_jsonsettingtest.moc"
