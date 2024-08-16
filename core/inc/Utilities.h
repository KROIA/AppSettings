#pragma once

#include "AppSettings_base.h"
#include <QVariant>
#include <QJsonValue>
#include <Logger.h>


namespace AppSettings
{
	QJsonValue APP_SETTINGS_EXPORT variantToJsonValue(const QVariant& var);
	QVariant APP_SETTINGS_EXPORT jsonValueToVariant(const QJsonValue& value);

	Log::LogObject APP_SETTINGS_EXPORT &logger();
}