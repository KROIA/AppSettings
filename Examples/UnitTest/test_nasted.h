#pragma once
#include "Settings.h"
#include "test.h"
#include <QObject>
#include <QCoreapplication>
#include "dummySignalReceiver.h"


class Test_nasted : public Test
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
	bool test1(TestResults& results)
	{
		TEST_START(results);

		class Group0 : public Settings::SettingsGroup
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

			Settings::Setting m_testSetting0 = Settings::Setting("Value0", 10);
			Settings::Setting m_testSetting1 = Settings::Setting("Value1", 11);
			Settings::Setting m_testSetting2 = Settings::Setting("Value2", 12);
			Settings::Setting m_testSetting3 = Settings::Setting("Value3", 13);
		};
		class Group1 : public Settings::SettingsGroup
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

			Settings::Setting m_testSetting0 = Settings::Setting("TestValue0", 0);
			Settings::Setting m_testSetting1 = Settings::Setting("TestValue1", 1);
			Settings::Setting m_testSetting2 = Settings::Setting("TestValue2", 2);
			Settings::Setting m_testSetting3 = Settings::Setting("TestValue3", 3);
		};

		class Group2 : public Settings::SettingsGroup
		{
		public:
			Group2()
				: SettingsGroup("Group2")
			{
				addSetting(m_testSetting0);
				addSetting(m_testSetting1);
				addSetting(m_testSetting2);
				addSetting(m_testSetting3);
				addGroup(m_group0);
				addGroup(m_group1);
			}

			Settings::Setting m_testSetting0 = Settings::Setting("Name1", "Peter");
			Settings::Setting m_testSetting1 = Settings::Setting("Name2", "Simon");
			Settings::Setting m_testSetting2 = Settings::Setting("Name3", "Kurt");
			Settings::Setting m_testSetting3 = Settings::Setting("Name4", "Hans");
			Group0 m_group0;
			Group1 m_group1;
		};

		class AppSettings : public Settings::ApplicationSettings
		{
		public:
			AppSettings()
				: ApplicationSettings("Test_nasted_test1")
			{
				setPath("settings");

				addGroup(m_group2);
			}

			Group2 m_group2;
		};

		AppSettings settings;
		
		TEST_ASSERT(settings.save());
		TEST_ASSERT(settings.read());

		AppSettings settings2;
		TEST_ASSERT(settings2.read());
		TEST_ASSERT(settings2.m_group2.m_group1.m_testSetting0.getValue() == 0);
		TEST_ASSERT(settings2.m_group2.m_group1.m_testSetting0.getName() == "TestValue0");
		TEST_ASSERT(settings2.m_group2.m_testSetting0.getValue() == "Peter");
		TEST_ASSERT(settings2.m_group2.m_testSetting0.getName() == "Name1");

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
				addGroup(m_group1);
			}

			Settings::Setting m_testSetting = Settings::Setting("Name", "Peter");
			Group1 m_group1;
		};

		class AppSettings : public Settings::ApplicationSettings
		{
		public:
			AppSettings()
				: ApplicationSettings("Test_nasted_test2")
			{
				setPath("settings");

				addGroup(m_group2);
			}

			Group2 m_group2;
		};


		AppSettings settings;
		DummySignalReceiver signalWatcher1;
		QObject::connect(&settings.m_group2.m_group1.m_testSetting, &Settings::Setting::valueChanged, &signalWatcher1, &DummySignalReceiver::onValueChanged);
		QObject::connect(&settings.m_group2.m_group1.m_testSetting, &Settings::Setting::nameChanged, &signalWatcher1, &DummySignalReceiver::onNameChanged);

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

		settings.m_group2.m_group1.m_testSetting = 5;
		QCoreApplication::processEvents();
		TEST_ASSERT(signalWatcher1.m_nameChanged == false);
		signalWatcher1.m_nameChanged = false;
		TEST_ASSERT(signalWatcher1.m_valueChanged == true);
		signalWatcher1.m_valueChanged = false;
		TEST_ASSERT(settings.save());

		AppSettings settings2;
		DummySignalReceiver signalWatcher2;
		QObject::connect(&settings2.m_group2.m_group1.m_testSetting, &Settings::Setting::valueChanged, &signalWatcher2, &DummySignalReceiver::onValueChanged);
		QObject::connect(&settings2.m_group2.m_group1.m_testSetting, &Settings::Setting::nameChanged, &signalWatcher2, &DummySignalReceiver::onNameChanged);

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

		TEST_ASSERT(settings2.m_group2.m_group1.m_testSetting.getValue() == 5);
		TEST_ASSERT(settings2.m_group2.m_group1.m_testSetting.getName() == "TestValue");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getValue() == "Peter");
		TEST_ASSERT(settings2.m_group2.m_testSetting.getName() == "Name");

		TEST_END;
	}

};
 