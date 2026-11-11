#pragma once
#include "AppSettings.h"
#include "ui/SettingsWidget.h"
#include "ui/DynamicInputWidget.h"
#include "UnitTest.h"
#include <QApplication>
#include <QToolButton>
#include <QKeySequence>


/// <summary>
/// Checks that the German UI texts with umlauts are decoded correctly.
/// The library is compiled with /utf-8 (MSVC); without it the umlauts would break.
/// The expected texts are built from code points, so this file stays ASCII.
/// </summary>
class Test_encoding : public UnitTest::Test
{
	TEST_CLASS(Test_encoding)
public:
	Test_encoding()
		: Test("Test_encoding")
	{
		ADD_TEST(Test_encoding::tooltips);
	}

private:
	// Returns true, if a QToolButton inside the widget has the tooltip
	static bool hasToolTip(const QWidget& widget, const QString& toolTip)
	{
		for (const QToolButton* button : widget.findChildren<QToolButton*>())
		{
			if (button->toolTip() == toolTip)
				return true;
		}
		return false;
	}

	TEST_FUNCTION(tooltips)
	{
		TEST_START;

		static int argc = 1;
		static char appName[] = "UnitTest";
		static char* argv[] = { appName, nullptr };
		QApplication app(argc, argv);

		// "Auf Standardwert zuruecksetzen" with ue = U+00FC
		const QString reset = QString("Auf Standardwert zur") + QChar(0x00FC) + QString("cksetzen");
		TEST_ASSERT(reset.length() == 29);

		AppSettings::UI::SettingsWidget settingsWidget;
		TEST_ASSERT(hasToolTip(settingsWidget, reset));

		AppSettings::Setting setting("A", 5);
		settingsWidget.setSetting(&setting);
		TEST_ASSERT(hasToolTip(settingsWidget, reset + ": 5"));

		// "Tastenkuerzel loeschen" with ue = U+00FC, oe = U+00F6
		const QString clear = QString("Tastenk") + QChar(0x00FC) + QString("rzel l") + QChar(0x00F6) + QString("schen");
		TEST_ASSERT(clear.length() == 20);

		AppSettings::UI::DynamicInputWidget inputWidget;
		inputWidget.setValue(QKeySequence("Ctrl+R"));
		TEST_ASSERT(hasToolTip(inputWidget, clear));
	}
};
