#pragma once

#include "AppSettings_base.h"
#include <QWidget>
#include <QLabel>



#include "DynamicInputWidget.h"


namespace AppSettings
{
	class ListSetting;
	namespace UI
	{
		class APP_SETTINGS_EXPORT ListSettingsWidget : public QWidget
		{
			Q_OBJECT

		public:
			ListSettingsWidget(QWidget* parent = nullptr);
			~ListSettingsWidget();

			void setSetting(ListSetting* setting);
			ListSetting* getSetting() const{ return m_setting;}

		signals:
			void valueChanged();

		public slots:
			void saveSetting();

		private slots:
			void onKeyChanged();
			void onValueChanged();

		private:
			ListSetting* m_setting = nullptr;

			QLabel *m_nameLabel;
			QWidget* m_contentWidget;
			bool m_hasChanges = false;
			
			class MapWidget
			{
			public:
				MapWidget(QVariant key, QVariant value);
				~MapWidget();

				DynamicInputWidget* key;
				DynamicInputWidget* value;
			};
			std::vector<MapWidget*> m_mapWidgets;
		};
	}
}