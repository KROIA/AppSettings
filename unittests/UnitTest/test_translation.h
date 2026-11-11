#pragma once
#include "AppSettings.h"
#include "UnitTest.h"
#include <QCoreApplication>


/// <summary>
/// Checks AppSettings::installTranslator() with the embedded translations.
/// The German texts are built from code points, so this file stays ASCII.
/// </summary>
class Test_translation : public UnitTest::Test
{
	TEST_CLASS(Test_translation)
public:
	Test_translation()
		: Test("Test_translation")
	{
		ADD_TEST(Test_translation::installTranslator);
	}

private:
	TEST_FUNCTION(installTranslator)
	{
		TEST_START;

		static int argc = 1;
		static char appName[] = "UnitTest";
		static char* argv[] = { appName, nullptr };
		QCoreApplication app(argc, argv);

		// "Ungueltige Einstellungen" with ue = U+00FC
		const QString german = QString("Ung") + QChar(0x00FC) + QString("ltige Einstellungen");
		const QString english = "Invalid settings";
		const char* context = "AppSettings::UI::UI_AppSettingsEditor";
		const QByteArray source = german.toUtf8();
		auto text = [&] { return QCoreApplication::translate(context, source.constData()); };

		TEST_ASSERT(AppSettings::installTranslator(QLocale(QLocale::English)));
		TEST_ASSERT(text() == english);
		TEST_ASSERT(QCoreApplication::translate("UI_AppSettingsEditor", "Speichern") == "Save");

		TEST_ASSERT(AppSettings::installTranslator(QLocale(QLocale::German)));
		TEST_ASSERT(text() == german);

		// Not available: English is used
		TEST_ASSERT(!AppSettings::installTranslator(QLocale(QLocale::French)));
		TEST_ASSERT(text() == english);

		TEST_ASSERT(AppSettings::installTranslator(QLocale(QLocale::German)));
		TEST_ASSERT(text() == german);
	}
};
