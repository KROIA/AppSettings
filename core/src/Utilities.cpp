#include "Utilities.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QKeySequence>

#include "CustomVariants/Selection.h"
#include "AppSettings_info.h"

#include <QCoreApplication>
#include <QTranslator>

// Q_INIT_RESOURCE must be used outside of a namespace.
// Needed for the static library, where the resource is not registered automatically.
static void initTranslationResource()
{
	Q_INIT_RESOURCE(appsettings_translations);
}


namespace AppSettings
{
	QJsonValue variantToJsonValue(const QVariant& var)
	{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
		switch (static_cast<QMetaType::Type>(var.type()))
#else
		switch (static_cast<QMetaType::Type>(var.typeId()))
#endif
		{
		case QMetaType::Bool:
			return QJsonValue(var.toBool());
		case QMetaType::Int:
			return QJsonValue(var.toInt());
		case QMetaType::Double:
			return QJsonValue(var.toDouble());
		case QMetaType::QString:
			return QJsonValue(var.toString());
		case QMetaType::QVariantList: {
			QJsonArray jsonArray;
			for (const QVariant& item : var.toList()) {
				jsonArray.append(variantToJsonValue(item));
			}
			return jsonArray;
		}
		case QMetaType::QVariantMap: {
			QJsonObject jsonObject;
			QVariantMap varMap = var.toMap();
			for (auto it = varMap.begin(); it != varMap.end(); ++it) {
				jsonObject.insert(it.key(), variantToJsonValue(it.value()));
			}
			return jsonObject;
		}
		case QMetaType::QStringList: {
			QJsonArray jsonArray;
			for (const QString& str : var.toStringList()) {
				jsonArray.append(QJsonValue(str));
			}
			return jsonArray;
		}
		case QMetaType::QByteArray:
			return QJsonValue(QString::fromUtf8(var.toByteArray()));
		case QMetaType::ULongLong:
			return QJsonValue(static_cast<double>(var.toULongLong())); // No native support for unsigned long long
		case QMetaType::QKeySequence:
			return QJsonValue(var.value<QKeySequence>().toString(QKeySequence::PortableText));
		default:
		{
			// Custom types
			if (var.canConvert<Selection>())
			{
				Selection selection = var.value<Selection>();
				return selection.toJson();
			}
			break;
		}
		}
		return QJsonValue(); // Null value for unsupported types
	}
	QVariant jsonValueToVariant(const QJsonValue& value)
	{
		switch (value.type()) 
		{
		case QJsonValue::Bool:
			return QVariant(value.toBool());
		case QJsonValue::Double:
			return QVariant(value.toDouble());
		case QJsonValue::String:
			return QVariant(value.toString());
		case QJsonValue::Array: {
			QList<QVariant> variantList;
			QJsonArray jsonArray = value.toArray();
			for (const QJsonValue& val : jsonArray) {
				variantList.append(jsonValueToVariant(val));
			}
			return QVariant(variantList);
		}
		case QJsonValue::Object: {
			// Check if the Object is a Selection
			Selection selection;
			if (selection.fromJson(value))
				return QVariant::fromValue(selection);

			QVariantMap variantMap;
			QJsonObject jsonObject = value.toObject();
			for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
				variantMap.insert(it.key(), jsonValueToVariant(it.value()));
			}
			return QVariant(variantMap);
		}
		case QJsonValue::Null:
		case QJsonValue::Undefined:
			return QVariant(); // Null QVariant
		default:
		{
			
			break;
		}
		}
		return QVariant(); // Unsupported types will return null
	}

	QVariant jsonValueToVariant(const QJsonValue& value, const QVariant& typeHint)
	{
		QVariant plain = jsonValueToVariant(value);
		if (!typeHint.isValid() || !plain.isValid() || plain.userType() == typeHint.userType())
			return plain;

		if (typeHint.userType() == QMetaType::QKeySequence && value.isString())
			return QVariant::fromValue(QKeySequence::fromString(value.toString(), QKeySequence::PortableText));

		QVariant converted = plain;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
		bool ok = converted.convert(typeHint.userType());
#else
		bool ok = converted.convert(typeHint.metaType());
#endif
		// A JSON number is always a double. Only accept the conversion if it is lossless,
		// so 2.5 never silently becomes 2 (or 3) and out of range values are not clamped.
		if (ok && value.isDouble())
			ok = converted.toDouble() == value.toDouble();
		if (!ok)
		{
			Logger::logWarning(std::string("Unable to convert loaded value to type ") + typeHint.typeName()
				+ ". Keeping the value as " + (plain.typeName() ? plain.typeName() : "unknown"));
			return plain;
		}
		return converted;
	}

	bool installTranslator(const QLocale& locale)
	{
		static QTranslator translator;
		initTranslationResource();
		QCoreApplication::removeTranslator(&translator);

		// The source texts are German
		if (locale.language() == QLocale::German)
			return true;

		const QString directory = ":/AppSettings/translations";
		bool found = translator.load(locale, "appsettings", "_", directory);
		if (!found && !translator.load(QLocale(QLocale::English), "appsettings", "_", directory))
			return false;
		QCoreApplication::installTranslator(&translator);
		return found;
	}
}
