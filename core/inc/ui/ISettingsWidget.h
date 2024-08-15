#pragma once

#include "AppSettings_base.h"
#include <QWidget>


namespace AppSettings
{
	class ISettingsWidget: public QWidget
	{
		Q_OBJECT
	public:
		ISettingsWidget(QWidget* parent = nullptr) : QWidget(parent) {}

		virtual void saveSetting() = 0;
	};
}