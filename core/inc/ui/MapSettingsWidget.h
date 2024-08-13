#pragma once

#include "AppSettings_base.h"
#include <QWidget>
#include <QLabel>


#include "MapSetting.h"
#include "DynamicInputWidget.h"


namespace AppSettings
{
	namespace UI
	{
		class MapSettingsWidget : public QWidget
		{
			Q_OBJECT

		public:
			MapSettingsWidget(QWidget* parent = nullptr);
			~MapSettingsWidget();

			void setSetting(MapSetting* setting);
			MapSetting* getSetting() const{ return m_setting;}

		signals:
			void valueChanged();

		public slots:
			void saveSetting();

		private slots:
			void onKeyChanged();
			void onValueChanged();

		private:
			MapSetting* m_setting = nullptr;

			//QHBoxLayout* m_layout;
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