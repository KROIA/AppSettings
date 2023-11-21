#include "Settings.h"
#include "test.h"

class Test_simple : public Test
{
public:
	Test_simple()
		: Test("Test_simple")
	{
		ADD_TEST(Test_simple::test1);

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

};
