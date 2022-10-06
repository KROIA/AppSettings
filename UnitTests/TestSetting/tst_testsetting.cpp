#include <QtTest>
#include "setting.h"
// add necessary includes here

class TestSetting : public QObject
{
        Q_OBJECT

    public:
        TestSetting();
        ~TestSetting();

    private slots:
        void test_case1();

};

TestSetting::TestSetting()
{

}

TestSetting::~TestSetting()
{

}

void TestSetting::test_case1()
{

}

QTEST_APPLESS_MAIN(TestSetting)

#include "tst_testsetting.moc"
