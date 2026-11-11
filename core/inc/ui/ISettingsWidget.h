#pragma once

#include "AppSettings_base.h"
#include "ISetting.h"
#include <QWidget>
#include <QToolButton>
#include <QStyle>


namespace AppSettings
{
	class APP_SETTINGS_API ISettingsWidget: public QWidget
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

		/// <summary>
		/// Checks if the setting's name or its saved value text (toString()) contains the text.
		/// The check ignores the case. An empty text always matches.
		/// </summary>
		/// <param name="text">The filter text</param>
		/// <returns>True, if the row should be shown</returns>
		virtual bool matchesFilter(const QString& text) const
		{
			if (text.isEmpty())
				return true;
			if (!m_filterSetting)
				return false;
			return m_filterSetting->getName().contains(text, Qt::CaseInsensitive)
				|| m_filterSetting->toString().contains(text, Qt::CaseInsensitive);
		}

	protected:
		/// <summary>
		/// The setting matchesFilter() checks. Set by the setSetting() of the derived widget.
		/// </summary>
		const ISetting* m_filterSetting = nullptr;

		/// <summary>
		/// Creates a small button which calls resetToDefault() when clicked
		/// </summary>
		/// <returns>The new button, owned by this widget</returns>
		QToolButton* createResetButton();
	};
}