#pragma once

#include "AppSettings_base.h"
#include <QLabel>
#include <QPushbutton>
#include <QGridLayout>

#include "ISettingsWidget.h"



namespace AppSettings
{
	class MapSetting;
	namespace UI
	{
		class DynamicInputWidget;
		class APP_SETTINGS_API MapSettingsWidget : public ISettingsWidget
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
			void saveSetting() override;

		private slots:
			void onKeyChanged();
			void onValueChanged();

			void onAddButtonClicked();
			void onRemoveButtonClicked();

		private:
			void replaceAddButton();

			MapSetting* m_setting = nullptr;

			QLabel *m_nameLabel;
			QWidget* m_contentWidget;
			bool m_hasChanges = false;
			QGridLayout *m_contentGridLayout = nullptr;
			QPushButton* m_addButton = nullptr;
			
			class MapWidget
			{
			public:
				MapWidget(QVariant key, QVariant value, bool removeButtonEnabled);
				~MapWidget();

				DynamicInputWidget* key;
				DynamicInputWidget* value;
				QPushButton* removeButton = nullptr;
			};
			std::vector<MapWidget*> m_mapWidgets;
		};
	}
}