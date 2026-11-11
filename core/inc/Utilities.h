#pragma once

#include "AppSettings_base.h"
#include <QVariant>
#include <QJsonValue>
#include <QLocale>
#include <Logger.h>


namespace AppSettings
{
	QJsonValue APP_SETTINGS_API variantToJsonValue(const QVariant& var);
	QVariant APP_SETTINGS_API jsonValueToVariant(const QJsonValue& value);

	/// <summary>
	/// Converts a JSON value to a QVariant of the same type as typeHint.
	/// A string is parsed as QKeySequence (PortableText) if the hint is a QKeySequence.
	/// Numbers are only converted if the conversion is lossless (an int stays an int).
	/// If the conversion fails, a warning is logged and the unconverted value is returned.
	/// </summary>
	/// <param name="value">The JSON value to convert</param>
	/// <param name="typeHint">A value of the wanted type. If invalid, this behaves like jsonValueToVariant(value)</param>
	/// <returns>The converted value</returns>
	QVariant APP_SETTINGS_API jsonValueToVariant(const QJsonValue& value, const QVariant& typeHint);

	/// <summary>
	/// Installs the translation of the library's UI texts for the locale.
	/// The source texts are German: for a German locale, the translator is removed and the German texts are shown.
	/// If the library has a translation for the locale, it is installed.
	/// Otherwise the English translation is installed as fallback.
	/// The library owns one translator. A new call replaces the translator of the previous call.
	/// Needs an existing QCoreApplication.
	/// </summary>
	/// <param name="locale">The wanted language. Default: the system locale</param>
	/// <returns>True, if the texts are shown in the wanted language.
	/// False, if the language is not available and English is used instead.</returns>
	APP_SETTINGS_API bool installTranslator(const QLocale& locale = QLocale());
}