#pragma once

#include "AppSettings_base.h"
#include <QWidget>
#include <QToolButton>
#include <QStyle>


namespace AppSettings
{
	class ISettingsWidget: public QWidget
	{
		Q_OBJECT
	public:
		ISettingsWidget(QWidget* parent = nullptr) : QWidget(parent) {}

		virtual void saveSetting() = 0;

		/// <summary>
		/// Shows the default value of the setting in the editor.
		/// The setting itself only changes when saveSetting() is called.
		/// </summary>
		virtual void resetToDefault() = 0;

	protected:
		/// <summary>
		/// Creates a small button which calls resetToDefault() when clicked
		/// </summary>
		/// <returns>The new button, owned by this widget</returns>
		QToolButton* createResetButton()
		{
			QToolButton* button = new QToolButton(this);
			button->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
			button->setToolTip("Reset to default");
			connect(button, &QToolButton::clicked, this, &ISettingsWidget::resetToDefault);
			return button;
		}
	};
}