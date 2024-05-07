#pragma once
#include "AppSettings.h"
#include "UnitTest.h"
#include <QObject>
#include <QCoreapplication>
#include "dummySignalReceiver.h"




class Test_simple : public UnitTest::Test
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
    TEST_FUNCTION(test1)
	{
        TEST_START;

		class Group1 : public AppSettings::SettingsGroup
		{
		public:
			Group1()
				: SettingsGroup("Group1")
			{
				addSetting(m_testSetting);
			}

			AppSettings::Setting m_testSetting = AppSettings::Setting("TestValue", 0);
		};

		class Group2 : public AppSettings::SettingsGroup
		{
		public:
			Group2()
				: SettingsGroup("Group2")
			{
				addSetting(m_testSetting);
			}

			AppSettings::Setting m_testSetting = AppSettings::Setting("Name", "Peter");
		};

		class ThisAppSettings : public AppSettings::ApplicationSettings
		{
		public:
			ThisAppSettings()
				: ApplicationSettings("Test_simple_test1")
			{
				setPath("settings");

				addGroup(m_group1);
				addGroup(m_group2);
			}

			Group1 m_group1;
			Group2 m_group2;
		};

		ThisAppSettings settings;
		
		TEST_ASSERT(settings.save());
		TEST_ASSERT(settings.load());

		ThisAppSettings settings2;
		TEST_ASSERT(settings2.load());
		TEST_ASSERT(settings2.m_group1.m_testSetting.getValue() == 0);
		TEST_ASSERT(settings2.m_group1.m_testSetting.getName() == "TestValue");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getValue() == "Peter");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getName() == "Name");
	}



    TEST_FUNCTION(test2)
	{
        TEST_START;


		class Group1 : public AppSettings::SettingsGroup
		{
		public:
			Group1()
				: SettingsGroup("Group1")
			{
				addSetting(m_testSetting);
			}

			AppSettings::Setting m_testSetting = AppSettings::Setting("TestValue", 0);
		};

		class Group2 : public AppSettings::SettingsGroup
		{
		public:
			Group2()
				: SettingsGroup("Group2")
			{
				addSetting(m_testSetting);
			}

			AppSettings::Setting m_testSetting = AppSettings::Setting("Name", "Peter");
		};

		class ThisAppSettings : public AppSettings::ApplicationSettings
		{
		public:
			ThisAppSettings()
				: ApplicationSettings("Test_simple_test2")
			{
				setPath("settings");

				addGroup(m_group1);
				addGroup(m_group2);
			}

			Group1 m_group1;
			Group2 m_group2;
		};

		ThisAppSettings settings;
		DummySignalReceiver signalWatcher1;
		QObject::connect(&settings.m_group1.m_testSetting, &AppSettings::Setting::valueChanged, &signalWatcher1, &DummySignalReceiver::onValueChanged);
		QObject::connect(&settings.m_group1.m_testSetting, &AppSettings::Setting::nameChanged, &signalWatcher1, &DummySignalReceiver::onNameChanged);

		TEST_ASSERT(settings.save());

		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher1.m_nameChanged == false);
		signalWatcher1.m_nameChanged = false;
		TEST_ASSERT(signalWatcher1.m_valueChanged == false);
		signalWatcher1.m_valueChanged = false;

		TEST_ASSERT(settings.load());

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

		ThisAppSettings settings2;
		DummySignalReceiver signalWatcher2;
		QObject::connect(&settings2.m_group1.m_testSetting, &AppSettings::Setting::valueChanged, &signalWatcher2, &DummySignalReceiver::onValueChanged);
		QObject::connect(&settings2.m_group1.m_testSetting, &AppSettings::Setting::nameChanged, &signalWatcher2, &DummySignalReceiver::onNameChanged);

		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher2.m_nameChanged == false);
		signalWatcher2.m_nameChanged = false;
		TEST_ASSERT(signalWatcher2.m_valueChanged == false);
		signalWatcher2.m_valueChanged = false;

		TEST_ASSERT(settings2.load());

		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher2.m_nameChanged == false);
		signalWatcher2.m_nameChanged = false;
		TEST_ASSERT(signalWatcher2.m_valueChanged == true);
		signalWatcher2.m_valueChanged = false;

		TEST_ASSERT(settings2.m_group1.m_testSetting.getValue() == 5);
		TEST_ASSERT(settings2.m_group1.m_testSetting.getName() == "TestValue");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getValue() == "Peter");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getName() == "Name");

		std::cout << settings2;
	}

};
 
