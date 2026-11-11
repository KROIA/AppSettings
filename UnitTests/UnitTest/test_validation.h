#pragma once
#include "AppSettings.h"
#include "UnitTest.h"
#include <QKeySequence>


/// <summary>
/// Checks SettingsGroup::setValidator() and SettingsGroup::validate().
/// </summary>
class Test_validation : public UnitTest::Test
{
	TEST_CLASS(Test_validation)
public:
	Test_validation()
		: Test("Test_validation")
	{
		ADD_TEST(Test_validation::noValidator);
		ADD_TEST(Test_validation::rejecting);
		ADD_TEST(Test_validation::childFromRoot);
		ADD_TEST(Test_validation::duplicates);
	}

private:
	class Shortcuts : public AppSettings::SettingsGroup
	{
	public:
		Shortcuts()
			: SettingsGroup("Shortcuts")
		{
			addSetting(m_rotate);
			addSetting(m_copy);
		}

		AppSettings::Setting m_rotate = AppSettings::Setting("Rotate", QKeySequence("Ctrl+R"));
		AppSettings::Setting m_copy = AppSettings::Setting("Copy", QKeySequence("Ctrl+C"));
	};

	class Root : public AppSettings::SettingsGroup
	{
	public:
		Root()
			: SettingsGroup("Root")
		{
			addGroup(m_shortcuts);
		}

		Shortcuts m_shortcuts;
	};

	// Rejects two settings of the group with the same value
	static QString rejectDuplicates(const AppSettings::SettingsGroup& group)
	{
		const std::vector<AppSettings::ISetting*>& settings = group.getSettings();
		for (size_t i = 0; i < settings.size(); ++i)
		{
			const AppSettings::Setting* a = dynamic_cast<const AppSettings::Setting*>(settings[i]);
			for (size_t j = i + 1; a && j < settings.size(); ++j)
			{
				const AppSettings::Setting* b = dynamic_cast<const AppSettings::Setting*>(settings[j]);
				if (b && a->getValue() == b->getValue())
					return a->getName() + " and " + b->getName() + " have the same value";
			}
		}
		return QString();
	}

	TEST_FUNCTION(noValidator)
	{
		TEST_START;

		Root root;
		TEST_ASSERT(root.validate().isEmpty());

		// Removing the validator works too
		root.setValidator([](const AppSettings::SettingsGroup&) { return QString("bad"); });
		root.setValidator(nullptr);
		TEST_ASSERT(root.validate().isEmpty());
	}

	TEST_FUNCTION(rejecting)
	{
		TEST_START;

		Shortcuts group;
		group.setValidator([](const AppSettings::SettingsGroup&) { return QString("Not allowed"); });
		TEST_ASSERT(group.validate() == "Not allowed");
	}

	TEST_FUNCTION(childFromRoot)
	{
		TEST_START;

		Root root;
		root.setValidator([](const AppSettings::SettingsGroup&) { return QString(); });
		root.m_shortcuts.setValidator([](const AppSettings::SettingsGroup&) { return QString("Child rejects"); });
		TEST_ASSERT(root.validate() == "Child rejects");
	}

	TEST_FUNCTION(duplicates)
	{
		TEST_START;

		Root root;
		root.m_shortcuts.setValidator(&Test_validation::rejectDuplicates);
		TEST_ASSERT(root.validate().isEmpty());

		root.m_shortcuts.m_copy.setValue(QKeySequence("Ctrl+R"));
		TEST_ASSERT(root.validate() == "Rotate and Copy have the same value");

		root.m_shortcuts.m_copy.setValue(QKeySequence("Ctrl+C"));
		TEST_ASSERT(root.validate().isEmpty());
	}
};
