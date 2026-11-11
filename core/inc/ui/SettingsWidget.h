#pragma once

#include "AppSettings_base.h"

#include <QLabel>
#include <QHBoxLayout>

#include "Setting.h"
#include "ISettingsWidget.h"



namespace AppSettings
{
	
	namespace UI
	{
		class DynamicInputWidget;
		class APP_SETTINGS_API SettingsWidget : public ISettingsWidget
		{
			Q_OBJECT

		public:
			SettingsWidget(QWidget* parent = nullptr);
			~SettingsWidget();

			void setSetting(Setting* setting);
			Setting* getSetting() const{ return m_setting;}

		signals:
			void valueChanged();

		public slots:
			void saveSetting() override;

			/// <summary>
			/// Shows the default value of the setting in the input field.
			/// The setting itself only changes when saveSetting() is called.
			/// </summary>
			void resetToDefault() override;

		private slots:
			void onValueChanged();

		private:
			Setting* m_setting;

			QHBoxLayout* m_layout;
			QLabel *m_nameLabel;
			DynamicInputWidget *m_inputWidget;
			QToolButton* m_resetButton;
			bool m_hasChanges = false;
			

		};
	}
}