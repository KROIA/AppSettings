#pragma once
#include "AppSettings.h"
#include "UnitTest.h"
#include <QObject>
#include <QCoreapplication>
#include "dummySignalReceiver.h"


class Test_nasted : public UnitTest::Test
{
	TEST_CLASS(Test_nasted)
public:
	Test_nasted()
		: Test("Test_nasted")
	{
		ADD_TEST(Test_nasted::test1);
		ADD_TEST(Test_nasted::test2);

	}
	
private:

    // Tests
    TEST_FUNCTION(test1)
	{
        TEST_START;

		class Group0 : public AppSettings::SettingsGroup
		{
		public:
			Group0()
				: SettingsGroup("Group0")
			{
				addSetting(m_testSetting0);
				addSetting(m_testSetting1);
				addSetting(m_testSetting2);
				addSetting(m_testSetting3);
			}

			AppSettings::Setting m_testSetting0 = AppSettings::Setting("Value0", 10);
			AppSettings::Setting m_testSetting1 = AppSettings::Setting("Value1", 11);
			AppSettings::Setting m_testSetting2 = AppSettings::Setting("Value2", 12);
			AppSettings::Setting m_testSetting3 = AppSettings::Setting("Value3", 13);
		};
		class Group1 : public AppSettings::SettingsGroup
		{
		public:
			Group1()
				: SettingsGroup("Group1")
			{
				addSetting(m_testSetting0);
				addSetting(m_testSetting1);
				addSetting(m_testSetting2);
				addSetting(m_testSetting3);
			}

			AppSettings::Setting m_testSetting0 = AppSettings::Setting("TestValue0", 0);
			AppSettings::Setting m_testSetting1 = AppSettings::Setting("TestValue1", 1);
			AppSettings::Setting m_testSetting2 = AppSettings::Setting("TestValue2", 2);
			AppSettings::Setting m_testSetting3 = AppSettings::Setting("TestValue3", 3);
		};

		class Group2 : public AppSettings::SettingsGroup
		{
		public:
			Group2()
				: SettingsGroup("Group2")
			{
				addSetting(m_testSetting0);
				addSetting(m_testSetting1);
				addSetting(m_testSetting2);
				addSetting(m_testSetting3);
				addSetting(m_testSetting4);
				addGroup(m_group0);
				addGroup(m_group1);
			}

			AppSettings::Setting m_testSetting0 = AppSettings::Setting("Name1", "Peter");
			AppSettings::Setting m_testSetting1 = AppSettings::Setting("Name2", "Simon");
			AppSettings::Setting m_testSetting2 = AppSettings::Setting("Name3", "Kurt");
			AppSettings::Setting m_testSetting3 = AppSettings::Setting("Name4", "Hans");
			AppSettings::Setting m_testSetting4 = AppSettings::Setting("Text", "Das ist ein Text mit mehreren\nZeilen");
			Group0 m_group0;
			Group1 m_group1;
		};

		class ThisAppSettings : public AppSettings::ApplicationSettings
		{
		public:
			ThisAppSettings()
				: ApplicationSettings("Test_nasted_test1")
			{
				setPath("settings");

				addGroup(m_group2);
			}
				
			Group2 m_group2;
		};

		ThisAppSettings settings;
		
		TEST_ASSERT(settings.save());
		TEST_ASSERT(settings.load());

		ThisAppSettings settings2;
		TEST_ASSERT(settings2.load());
		TEST_ASSERT(settings2.m_group2.m_group1.m_testSetting0.getValue() == 0);
		TEST_ASSERT(settings2.m_group2.m_group1.m_testSetting0.getName() == "TestValue0");
		TEST_ASSERT(settings2.m_group2.m_testSetting0.getValue() == "Peter");
		TEST_ASSERT(settings2.m_group2.m_testSetting0.getName() == "Name1");
		TEST_ASSERT(settings2.m_group2.m_testSetting4.getValue() == "Das ist ein Text mit mehreren\nZeilen");


		
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
				addGroup(m_group1);
			}

			AppSettings::Setting m_testSetting = AppSettings::Setting("Name", "Peter");
			Group1 m_group1;
		};

		class ThisAppSettings : public AppSettings::ApplicationSettings
		{
		public:
			ThisAppSettings()
				: ApplicationSettings("Test_nasted_test2")
			{
				setPath("settings");

				addGroup(m_group2);
			}

			Group2 m_group2;
		};


		ThisAppSettings settings;
		DummySignalReceiver signalWatcher1;
		QObject::connect(&settings.m_group2.m_group1.m_testSetting, &AppSettings::Setting::valueChanged, &signalWatcher1, &DummySignalReceiver::onValueChanged);
		QObject::connect(&settings.m_group2.m_group1.m_testSetting, &AppSettings::Setting::nameChanged, &signalWatcher1, &DummySignalReceiver::onNameChanged);

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

		settings.m_group2.m_group1.m_testSetting = 5;
		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher1.m_nameChanged == false);
		signalWatcher1.m_nameChanged = false;
		TEST_ASSERT(signalWatcher1.m_valueChanged == true);
		signalWatcher1.m_valueChanged = false;
		TEST_ASSERT(settings.save());

		ThisAppSettings settings2;
		DummySignalReceiver signalWatcher2;
		QObject::connect(&settings2.m_group2.m_group1.m_testSetting, &AppSettings::Setting::valueChanged, &signalWatcher2, &DummySignalReceiver::onValueChanged);
		QObject::connect(&settings2.m_group2.m_group1.m_testSetting, &AppSettings::Setting::nameChanged, &signalWatcher2, &DummySignalReceiver::onNameChanged);

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

		TEST_ASSERT(settings2.m_group2.m_group1.m_testSetting.getValue() == 5);
		TEST_ASSERT(settings2.m_group2.m_group1.m_testSetting.getName() == "TestValue");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getValue() == "Peter");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getName() == "Name");

		const AppSettings::ISetting* const peterSetting = settings2.m_group2.getSetting("Name");
		TEST_ASSERT(peterSetting != nullptr);

		std::cout << settings2.m_group2;
	}

};
 
