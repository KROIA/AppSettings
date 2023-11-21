
int main()
{
    return 0;
}
/*
#include <QtTest>
#include "Setting.h"
#include "SettingGroup.h"
#include <QSignalSpy>
using namespace Settings;
// add necessary includes here

class TestSetting : public QObject
{
        Q_OBJECT

    public:
        TestSetting();
        ~TestSetting();

    private slots:
        void test_copy();
        void test_equal();
        void test_setGet();
        void test_toString();
        void test_nameChange();

};

TestSetting::TestSetting()
{

}

TestSetting::~TestSetting()
{

}

void TestSetting::test_copy()
{
    Setting setting1("setting1","value1");
    QCOMPARE(setting1.getName(),"setting1");
    QCOMPARE(setting1.getValue(),"value1");
    QVERIFY(setting1.getParent() == nullptr);


    Setting setting2(setting1);
    QCOMPARE(setting2.getName(),"setting1");
    QCOMPARE(setting2.getValue(),"value1");
    QVERIFY(setting2.getParent() == nullptr);

    setting1.setValue(123);
    QCOMPARE(setting1.getName(),"setting1");
    QCOMPARE(setting1.getValue(),123);
    QVERIFY(setting1.getParent() == nullptr);
    QCOMPARE(setting2.getName(),"setting1");
    QCOMPARE(setting2.getValue(),"value1");
    QVERIFY(setting2.getParent() == nullptr);

    Setting setting3(setting1.getPair());
    QCOMPARE(setting3.getName(),"setting1");
    QCOMPARE(setting3.getValue(),123);
    QVERIFY(setting3.getParent() == nullptr);

}
void TestSetting::test_equal()
{
    Setting setting1("setting1","value1");

    Setting setting2("setting2","value2");
    QCOMPARE(setting2.getName(),"setting2");
    QCOMPARE(setting2.getValue(),"value2");
    QVERIFY(setting2.getParent() == nullptr);

    setting2 = setting1;
    QCOMPARE(setting2.getName(),"setting1");
    QCOMPARE(setting2.getValue(),"value1");
    QVERIFY(setting2.getParent() == nullptr);

    Setting setting3("setting3","");
    setting3 = setting1.getValue();
    QCOMPARE(setting3.getName(),"setting3");
    QCOMPARE(setting3.getValue(),"value1");
    QVERIFY(setting3.getParent() == nullptr);

}
void TestSetting::test_setGet()
{
    Setting setting1("setting1","value1");
    QSignalSpy spyValue(&setting1, &Setting::valueChanged);

    QCOMPARE(setting1.getName(),"setting1");
    QCOMPARE(setting1.getValue(),"value1");
    QCOMPARE(setting1.getPair(),(std::pair<QString, QVariant>("setting1","value1")));

    setting1.setValue(5);
    QCOMPARE(setting1.getName(),"setting1");
    QCOMPARE(setting1.getValue(),5);
    QCOMPARE(setting1.getPair(),(std::pair<QString, QVariant>("setting1",5)));

    setting1.setName("other");
    QCOMPARE(setting1.getName(),"other");
    QCOMPARE(setting1.getValue(),5);
    QCOMPARE(setting1.getPair(),(std::pair<QString, QVariant>("other",5)));

    Setting setting2(setting1);
    setting2.setName("test");
    QVERIFY(setting1 == setting2);
    QVERIFY(setting1 == setting2.getValue());
    QVERIFY(!(setting1 != setting2));
    QVERIFY(!(setting1 != setting2.getValue()));

    setting2.setValue("otherVal");
    QVERIFY(!(setting1 == setting2));
    QVERIFY(!(setting1 == setting2.getValue()));
    QVERIFY(setting1 != setting2);
    QVERIFY(setting1 != setting2.getValue());

    QCOMPARE(spyValue.count(), 1);

}
void TestSetting::test_toString()
{
    Setting setting1("setting1","value1");
    QString str = setting1.toString();
    QVERIFY(str.indexOf("setting1") != -1);
    QVERIFY(str.indexOf("value1") != -1);
}
void TestSetting::test_nameChange()
{
    Setting setting1("setting1","value1");
    QSignalSpy spy(&setting1, &Setting::nameChanged);
    setting1.setName("newName");
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

    setting1.setName("newName");
    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time
}

QTEST_APPLESS_MAIN(TestSetting)

#include "tst_testsetting.moc"
*/