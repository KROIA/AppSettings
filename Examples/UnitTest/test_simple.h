#pragma once
#include "Settings.h"
#include "test.h"
#include <QObject>
#include <QCoreapplication>

class TestQObject : public QObject
{
	Q_OBJECT
public:
	TestQObject()
		: QObject()
	{
		
	}
	QVariant m_value;
	QString m_name;
	bool m_valueChanged = false;
	bool m_nameChanged = false;
public slots:
	void onValueChanged(const QVariant& value)
	{
		m_value = value;
		m_valueChanged = true;
	}
	void onNameChanged(const QString& name)
	{
		m_name = name;	
		m_nameChanged = true;
	}


};


class Test_simple : public Test
{
	TEST_CLASS(Test_simple)
public:
	Test_simple()
		: Test("Test_simple")
	{
		ADD_TEST(Test_simple::test1);
		ADD_TEST(Test_simple::test2);

	}
	
private:

	// Tests
	bool test1(TestResults& results)
	{
		TEST_START(results);

		class Group1 : public Settings::SettingsGroup
		{
		public:
			Group1()
				: SettingsGroup("Group1")
			{
				addSetting(m_testSetting);
			}

			Settings::Setting m_testSetting = Settings::Setting("TestValue", 0);
		};

		class Group2 : public Settings::SettingsGroup
		{
		public:
			Group2()
				: SettingsGroup("Group2")
			{
				addSetting(m_testSetting);
			}

			Settings::Setting m_testSetting = Settings::Setting("Name", "Peter");
		};

		class AppSettings : public Settings::ApplicationSettings
		{
		public:
			AppSettings()
				: ApplicationSettings("Test_simple_test1")
			{
				setPath("settings");

				addGroup(m_group1);
				addGroup(m_group2);
			}

			Group1 m_group1;
			Group2 m_group2;
		};

		AppSettings settings;
		
		TEST_ASSERT(settings.save());
		TEST_ASSERT(settings.read());

		AppSettings settings2;
		TEST_ASSERT(settings2.read());
		TEST_ASSERT(settings2.m_group1.m_testSetting.getValue() == 0);
		TEST_ASSERT(settings2.m_group1.m_testSetting.getName() == "TestValue");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getValue() == "Peter");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getName() == "Name");

		TEST_END;
	}




	bool test2(TestResults& results)
	{
		TEST_START(results);


		class Group1 : public Settings::SettingsGroup
		{
		public:
			Group1()
				: SettingsGroup("Group1")
			{
				addSetting(m_testSetting);
			}

			Settings::Setting m_testSetting = Settings::Setting("TestValue", 0);
		};

		class Group2 : public Settings::SettingsGroup
		{
		public:
			Group2()
				: SettingsGroup("Group2")
			{
				addSetting(m_testSetting);
			}

			Settings::Setting m_testSetting = Settings::Setting("Name", "Peter");
		};

		class AppSettings : public Settings::ApplicationSettings
		{
		public:
			AppSettings()
				: ApplicationSettings("Test_simple_test2")
			{
				setPath("settings");

				addGroup(m_group1);
				addGroup(m_group2);
			}

			Group1 m_group1;
			Group2 m_group2;
		};

		AppSettings settings;
		TestQObject signalWatcher1;
		QObject::connect(&settings.m_group1.m_testSetting, &Settings::Setting::valueChanged, &signalWatcher1, &TestQObject::onValueChanged);
		QObject::connect(&settings.m_group1.m_testSetting, &Settings::Setting::nameChanged, &signalWatcher1, &TestQObject::onNameChanged);

		TEST_ASSERT(settings.save());

		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher1.m_nameChanged == false);
		signalWatcher1.m_nameChanged = false;
		TEST_ASSERT(signalWatcher1.m_valueChanged == false);
		signalWatcher1.m_valueChanged = false;

		TEST_ASSERT(settings.read());

		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher1.m_nameChanged == false);
		signalWatcher1.m_nameChanged = false;
		TEST_ASSERT(signalWatcher1.m_valueChanged == false);
		signalWatcher1.m_valueChanged = false;

		settings.m_group1.m_testSetting = 5;
		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher1.m_nameChanged == false);
		signalWatcher1.m_nameChanged = false;
		TEST_ASSERT(signalWatcher1.m_valueChanged == true);
		signalWatcher1.m_valueChanged = false;
		TEST_ASSERT(settings.save());

		AppSettings settings2;
		TestQObject signalWatcher2;
		QObject::connect(&settings2.m_group1.m_testSetting, &Settings::Setting::valueChanged, &signalWatcher2, &TestQObject::onValueChanged);
		QObject::connect(&settings2.m_group1.m_testSetting, &Settings::Setting::nameChanged, &signalWatcher2, &TestQObject::onNameChanged);

		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher2.m_nameChanged == false);
		signalWatcher2.m_nameChanged = false;
		TEST_ASSERT(signalWatcher2.m_valueChanged == false);
		signalWatcher2.m_valueChanged = false;

		TEST_ASSERT(settings2.read());

		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher2.m_nameChanged == false);
		signalWatcher2.m_nameChanged = false;
		TEST_ASSERT(signalWatcher2.m_valueChanged == true);
		signalWatcher2.m_valueChanged = false;

		TEST_ASSERT(settings2.m_group1.m_testSetting.getValue() == 5);
		TEST_ASSERT(settings2.m_group1.m_testSetting.getName() == "TestValue");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getValue() == "Peter");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getName() == "Name");

		TEST_END;
	}

};
 