#pragma once
#include "AppSettings.h"
#include "UnitTest.h"
#include <QKeySequence>


/// <summary>
/// Checks default values, isDefault() and resetToDefault().
/// </summary>
class Test_defaults : public UnitTest::Test
{
	TEST_CLASS(Test_defaults)
public:
	Test_defaults()
		: Test("Test_defaults")
	{
		ADD_TEST(Test_defaults::setting);
		ADD_TEST(Test_defaults::listSetting);
		ADD_TEST(Test_defaults::mapSetting);
		ADD_TEST(Test_defaults::copy);
		ADD_TEST(Test_defaults::group);
		ADD_TEST(Test_defaults::saveResetLoad);
	}

private:
	class SubGroup : public AppSettings::SettingsGroup
	{
	public:
		SubGroup()
			: SettingsGroup("SubGroup")
		{
			addSetting(m_value);
		}

		AppSettings::Setting m_value = AppSettings::Setting("Value", 1.5);
	};

	class Group : public AppSettings::SettingsGroup
	{
	public:
		Group()
			: SettingsGroup("Group")
		{
			addSetting(m_key);
			addSetting(m_int);
			addSetting(m_list);
			addSetting(m_map);
			addGroup(m_sub);
		}

		AppSettings::Setting m_key = AppSettings::Setting("Key", QKeySequence("Ctrl+R"));
		AppSettings::Setting m_int = AppSettings::Setting("Int", 42);
		AppSettings::ListSetting m_list = AppSettings::ListSetting("List", { 1, "Two" });
		AppSettings::MapSetting m_map = AppSettings::MapSetting("Map", { {"A", 1}, {"B", 2} });
		SubGroup m_sub;
	};

	class DefaultSettings : public AppSettings::ApplicationSettings
	{
	public:
		DefaultSettings()
			: ApplicationSettings("Test_defaults")
		{
			addGroup(m_group);
		}

		Group m_group;
	};

	// Counts the valueChanged() signals of a setting
	static void countSignals(AppSettings::ISetting& setting, int& counter)
	{
		counter = 0;
		QObject::connect(&setting, &AppSettings::ISetting::valueChanged, [&counter] { ++counter; });
	}

	TEST_FUNCTION(setting)
	{
		TEST_START;

		AppSettings::Setting setting("Int", 42);
		TEST_ASSERT(setting.isDefault());
		TEST_ASSERT(setting.getDefaultValue() == 42);

		setting.setValue(7);
		TEST_ASSERT(!setting.isDefault());
		TEST_ASSERT(setting.getDefaultValue() == 42);
		setting = QVariant(8);
		TEST_ASSERT(setting.getDefaultValue() == 42);

		int counter;
		countSignals(setting, counter);
		setting.resetToDefault();
		TEST_ASSERT(counter == 1);
		TEST_ASSERT(setting.isDefault());
		TEST_ASSERT(setting.getValue() == 42);

		AppSettings::Setting key("Key", QKeySequence("Ctrl+R"));
		key.setValue(QKeySequence("Ctrl+X"));
		TEST_ASSERT(!key.isDefault());
		key.resetToDefault();
		TEST_ASSERT(key.isDefault());
		TEST_ASSERT(key.getValue().value<QKeySequence>() == QKeySequence("Ctrl+R"));
	}

	TEST_FUNCTION(listSetting)
	{
		TEST_START;

		AppSettings::ListSetting list("List", { 1, "Two" });
		TEST_ASSERT(list.isDefault());

		list.push_back(3);
		TEST_ASSERT(!list.isDefault());
		TEST_ASSERT(list.getDefaultData().size() == 2);

		int counter;
		countSignals(list, counter);
		list.resetToDefault();
		TEST_ASSERT(counter == 1);
		TEST_ASSERT(list.isDefault());
		TEST_ASSERT(list.size() == 2);
		TEST_ASSERT(list[0] == 1);
		TEST_ASSERT(list[1] == "Two");
	}

	TEST_FUNCTION(mapSetting)
	{
		TEST_START;

		AppSettings::MapSetting map("Map", { {"A", 1}, {"B", 2} });
		TEST_ASSERT(map.isDefault());

		map.set("A", 5);
		TEST_ASSERT(!map.isDefault());
		map.addPair("C", 3);
		TEST_ASSERT(map.getDefaultMap().size() == 2);

		int counter;
		countSignals(map, counter);
		map.resetToDefault();
		TEST_ASSERT(counter == 1);
		TEST_ASSERT(map.isDefault());
		TEST_ASSERT(map.size() == 2);
		TEST_ASSERT(map["A"] == 1);
		TEST_ASSERT(map["B"] == 2);

		// setMap replaces the content
		map.setMap({ {"A", 9} });
		TEST_ASSERT(map.size() == 1);
		TEST_ASSERT(map["A"] == 9);
	}

	TEST_FUNCTION(copy)
	{
		TEST_START;

		AppSettings::Setting setting("Int", 42);
		setting.setValue(7);
		AppSettings::Setting copied(setting);
		TEST_ASSERT(copied.getValue() == 7);
		TEST_ASSERT(copied.getDefaultValue() == 42);
		AppSettings::Setting assigned("Other", 0);
		assigned = setting;
		TEST_ASSERT(assigned.getDefaultValue() == 42);

		AppSettings::ListSetting list("List", { 1 });
		list.push_back(2);
		AppSettings::ListSetting copiedList(list);
		TEST_ASSERT(copiedList.getDefaultData().size() == 1);
		AppSettings::ListSetting assignedList;
		assignedList = list;
		TEST_ASSERT(assignedList.getDefaultData().size() == 1);

		AppSettings::MapSetting map("Map", { {"A", 1} });
		map.addPair("B", 2);
		AppSettings::MapSetting copiedMap(map);
		TEST_ASSERT(copiedMap.getDefaultMap().size() == 1);
		AppSettings::MapSetting assignedMap;
		assignedMap = map;
		TEST_ASSERT(assignedMap.getDefaultMap().size() == 1);
	}

	TEST_FUNCTION(group)
	{
		TEST_START;

		Group group;
		group.m_int.setValue(1);
		group.m_sub.m_value.setValue(9.0);

		group.resetToDefault(false);
		TEST_ASSERT(group.m_int.isDefault());
		TEST_ASSERT(!group.m_sub.m_value.isDefault());

		group.m_int.setValue(1);
		group.m_key.setValue(QKeySequence());
		group.m_list.clear();
		group.m_map.clear();
		group.resetToDefault();
		TEST_ASSERT(group.m_int.isDefault());
		TEST_ASSERT(group.m_key.isDefault());
		TEST_ASSERT(group.m_list.isDefault());
		TEST_ASSERT(group.m_map.isDefault());
		TEST_ASSERT(group.m_sub.m_value.isDefault());
	}

	TEST_FUNCTION(saveResetLoad)
	{
		TEST_START;

		DefaultSettings settings;
		settings.m_group.m_key.setValue(QKeySequence("Ctrl+X"));
		settings.m_group.m_int.setValue(7);
		QJsonObject changed = settings.saveToJson();

		settings.m_group.resetToDefault();
		QJsonObject reset = settings.saveToJson();

		DefaultSettings loaded;
		TEST_ASSERT(loaded.loadFromJson(changed));
		TEST_ASSERT(!loaded.m_group.m_key.isDefault());
		TEST_ASSERT(loaded.loadFromJson(reset));
		TEST_ASSERT(loaded.m_group.m_key.getValue().userType() == QMetaType::QKeySequence);
		TEST_ASSERT(loaded.m_group.m_key.getValue().value<QKeySequence>() == QKeySequence("Ctrl+R"));
		TEST_ASSERT(loaded.m_group.m_key.isDefault());
		TEST_ASSERT(loaded.m_group.m_int.getValue().userType() == QMetaType::Int);
		TEST_ASSERT(loaded.m_group.m_int.isDefault());
		TEST_ASSERT(loaded.m_group.m_list.isDefault());
		TEST_ASSERT(loaded.m_group.m_map.isDefault());
		TEST_ASSERT(loaded.m_group.m_sub.m_value.isDefault());
	}
};
