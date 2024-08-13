#pragma once
#include <QObject>
#include <QCoreapplication>
#include <QVariant>
#include "Setting.h"

class DummySignalReceiver : public QObject
{
	Q_OBJECT
public:
	DummySignalReceiver()
		: QObject()
	{

	}
	QVariant m_value;
	QString m_name;
	bool m_valueChanged = false;
	bool m_nameChanged = false;
public slots:
	void onValueChanged()
	{
		m_value = dynamic_cast<AppSettings::ISetting*>(QObject::sender())->toString();
		m_valueChanged = true;
	}
	void onNameChanged()
	{
		m_name = dynamic_cast<AppSettings::ISetting*>(QObject::sender())->getName();
		m_nameChanged = true;
	}


};