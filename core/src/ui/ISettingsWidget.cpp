#include "ui/ISettingsWidget.h"

namespace AppSettings
{
	QToolButton* ISettingsWidget::createResetButton()
	{
		QToolButton* button = new QToolButton(this);
		button->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
		button->setToolTip(tr("Auf Standardwert zurücksetzen"));
		connect(button, &QToolButton::clicked, this, &ISettingsWidget::resetToDefault);
		return button;
	}
}
