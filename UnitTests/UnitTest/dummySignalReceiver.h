#pragma once
#include <QObject>
#include <QCoreapplication>
#include <QVariant>

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
	void onValueChanged(const QVariant& value)
	{
		m_value = value;
		m_valueChanged = true;
	}
	void onNameChanged(const QString& name)
	{
		m_name = name;
		m_nameChanged = true;
	}


};