#include "Utilities.h"
#include <QJsonArray>
#include <QJsonObject>

#include "CustomVariants/Selection.h"
#include "AppSettings_info.h"


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

}
