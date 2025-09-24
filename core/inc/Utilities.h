#pragma once

#include "AppSettings_base.h"
#include <QVariant>
#include <QJsonValue>
#include <Logger.h>


namespace AppSettings
{
	QJsonValue APP_SETTINGS_API variantToJsonValue(const QVariant& var);
	QVariant APP_SETTINGS_API jsonValueToVariant(const QJsonValue& value);
}