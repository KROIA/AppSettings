#include "Utilities.h"
#include <QJsonArray>
#include <QJsonObject>


namespace AppSettings
{
	QJsonValue variantToJsonValue(const QVariant& var)
	{
		switch (var.type())
		{
		case QVariant::Bool:
			return QJsonValue(var.toBool());
		case QVariant::Int:
			return QJsonValue(var.toInt());
		case QVariant::Double:
			return QJsonValue(var.toDouble());
		case QVariant::String:
			return QJsonValue(var.toString());
		case QVariant::List: {
			QJsonArray jsonArray;
			for (const QVariant& item : var.toList()) {
				jsonArray.append(variantToJsonValue(item));
			}
			return jsonArray;
		}
		case QVariant::Map: {
			QJsonObject jsonObject;
			QVariantMap varMap = var.toMap();
			for (auto it = varMap.begin(); it != varMap.end(); ++it) {
				jsonObject.insert(it.key(), variantToJsonValue(it.value()));
			}
			return jsonObject;
		}
		case QVariant::StringList: {
			QJsonArray jsonArray;
			for (const QString& str : var.toStringList()) {
				jsonArray.append(QJsonValue(str));
			}
			return jsonArray;
		}
		case QVariant::ByteArray:
			return QJsonValue(QString::fromUtf8(var.toByteArray()));
		case QVariant::ULongLong:
			return QJsonValue(static_cast<double>(var.toULongLong())); // No native support for unsigned long long
		default:
			break;
		}
		return QJsonValue(); // Null value for unsupported types
	}
	QVariant jsonValueToVariant(const QJsonValue& value)
	{
		switch (value.type()) {
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
			break;
		}
		return QVariant(); // Unsupported types will return null
	}
}
