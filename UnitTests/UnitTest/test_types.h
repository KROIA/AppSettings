#pragma once
#include "AppSettings.h"
#include "Utilities.h"
#include "UnitTest.h"
#include <QKeySequence>
#include <QJsonDocument>


/// <summary>
/// Checks that values keep their type after a save/load round trip.
/// </summary>
class Test_types : public UnitTest::Test
{
	TEST_CLASS(Test_types)
public:
	Test_types()
		: Test("Test_types")
	{
		ADD_TEST(Test_types::keySequence);
		ADD_TEST(Test_types::numbers);
		ADD_TEST(Test_types::listAndMap);
	}

private:
	class Group : public AppSettings::SettingsGroup
	{
	public:
		Group()
			: SettingsGroup("Group")
		{
			addSetting(m_key);
			addSetting(m_chord);
			addSetting(m_empty);
			addSetting(m_int);
			addSetting(m_double);
			addSetting(m_bool);
			addSetting(m_list);
			addSetting(m_map);
		}

		AppSettings::Setting m_key = AppSettings::Setting("Key", QKeySequence("Ctrl+R"));
		AppSettings::Setting m_chord = AppSettings::Setting("Chord", QKeySequence("Ctrl+K, Ctrl+C"));
		AppSettings::Setting m_empty = AppSettings::Setting("Empty", QKeySequence());
		AppSettings::Setting m_int = AppSettings::Setting("Int", 42);
		AppSettings::Setting m_double = AppSettings::Setting("Double", 2.5);
		AppSettings::Setting m_bool = AppSettings::Setting("Bool", true);
		AppSettings::ListSetting m_list = AppSettings::ListSetting("List", { 7, "Text" });
		AppSettings::MapSetting m_map = AppSettings::MapSetting("Map", { {"A", 3}, {4, "B"} });
	};

	class TypeSettings : public AppSettings::ApplicationSettings
	{
	public:
		TypeSettings()
			: ApplicationSettings("Test_types")
		{
			addGroup(m_group);
		}

		Group m_group;
	};

	TEST_FUNCTION(keySequence)
	{
		TEST_START;

		TypeSettings saved;
		QJsonObject json = saved.saveToJson();
		QByteArray text = QJsonDocument(json).toJson();
		TEST_ASSERT(text.contains("\"Ctrl+R\""));
		TEST_ASSERT(text.contains("\"Ctrl+K, Ctrl+C\""));

		TypeSettings loaded;
		loaded.m_group.m_key.setValue(QKeySequence("Ctrl+X"));
		loaded.m_group.m_chord.setValue(QKeySequence("Ctrl+X"));
		loaded.m_group.m_empty.setValue(QKeySequence("Ctrl+X"));
		TEST_ASSERT(loaded.loadFromJson(json));

		TEST_ASSERT(loaded.m_group.m_key.getValue().userType() == QMetaType::QKeySequence);
		TEST_ASSERT(loaded.m_group.m_key.getValue().value<QKeySequence>() == QKeySequence("Ctrl+R"));
		TEST_ASSERT(loaded.m_group.m_chord.getValue().userType() == QMetaType::QKeySequence);
		TEST_ASSERT(loaded.m_group.m_chord.getValue().value<QKeySequence>() == QKeySequence("Ctrl+K, Ctrl+C"));
		TEST_ASSERT(loaded.m_group.m_empty.getValue().userType() == QMetaType::QKeySequence);
		TEST_ASSERT(loaded.m_group.m_empty.getValue().value<QKeySequence>().isEmpty());
	}

	TEST_FUNCTION(numbers)
	{
		TEST_START;

		TypeSettings saved;
		QJsonObject json = saved.saveToJson();

		// Different values, so load really changes them
		TypeSettings loaded;
		loaded.m_group.m_int.setValue(1);
		loaded.m_group.m_double.setValue(1.0);
		loaded.m_group.m_bool.setValue(false);
		TEST_ASSERT(loaded.loadFromJson(json));

		TEST_ASSERT(loaded.m_group.m_int.getValue().userType() == QMetaType::Int);
		TEST_ASSERT(loaded.m_group.m_int.getValue().toInt() == 42);
		TEST_ASSERT(loaded.m_group.m_double.getValue().userType() == QMetaType::Double);
		TEST_ASSERT(loaded.m_group.m_double.getValue().toDouble() == 2.5);
		TEST_ASSERT(loaded.m_group.m_bool.getValue().userType() == QMetaType::Bool);
		TEST_ASSERT(loaded.m_group.m_bool.getValue().toBool() == true);

		// A non integral number for an int setting is kept as double, not truncated
		QVariant kept = AppSettings::jsonValueToVariant(QJsonValue(2.5), QVariant(1));
		TEST_ASSERT(kept.userType() == QMetaType::Double);
		TEST_ASSERT(kept.toDouble() == 2.5);

		// Negative integral numbers stay exact
		QVariant negative = AppSettings::jsonValueToVariant(QJsonValue(-7.0), QVariant(1));
		TEST_ASSERT(negative.userType() == QMetaType::Int);
		TEST_ASSERT(negative.toInt() == -7);
	}

	TEST_FUNCTION(listAndMap)
	{
		TEST_START;

		TypeSettings saved;
		QJsonObject json = saved.saveToJson();

		TypeSettings loaded;
		loaded.m_group.m_list.set(0, 0);
		loaded.m_group.m_map["A"] = 0;
		TEST_ASSERT(loaded.loadFromJson(json));

		TEST_ASSERT(loaded.m_group.m_list.size() == 2);
		TEST_ASSERT(loaded.m_group.m_list[0].userType() == QMetaType::Int);
		TEST_ASSERT(loaded.m_group.m_list[0].toInt() == 7);
		TEST_ASSERT(loaded.m_group.m_list[1] == "Text");

		TEST_ASSERT(loaded.m_group.m_map.size() == 2);
		TEST_ASSERT(loaded.m_group.m_map["A"].userType() == QMetaType::Int);
		TEST_ASSERT(loaded.m_group.m_map["A"].toInt() == 3);
		const auto& map = loaded.m_group.m_map.getMap();
		bool intKeyFound = false;
		for (const auto& pair : map)
		{
			if (pair.first.userType() == QMetaType::Int && pair.first.toInt() == 4)
				intKeyFound = pair.second == "B";
		}
		TEST_ASSERT(intKeyFound);
	}
};
