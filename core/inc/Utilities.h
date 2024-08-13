#pragma once

#include "AppSettings_base.h"
#include <QVariant>
#include <QJsonValue>

namespace AppSettings
{
	QJsonValue APP_SETTINGS_EXPORT variantToJsonValue(const QVariant& var);
	QVariant APP_SETTINGS_EXPORT jsonValueToVariant(const QJsonValue& value);
}