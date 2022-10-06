#pragma once

#include <QTest>
#include <QSignalSpy>

class SubTest : public QObject
{
    Q_OBJECT
    public:
        SubTest(){};
    protected:
       // virtual void setup() = 0;
       // virtual void run() = 0;
       // virtual void cleanup() = 0;
    private:

};
