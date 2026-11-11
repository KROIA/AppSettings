#pragma once

#include "AppSettings_base.h"
#include <QVariant>
#include <QJsonValue>
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
}