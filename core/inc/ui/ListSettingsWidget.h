#pragma once

#include "AppSettings_base.h"
#include <QLabel>
#include <QPushButton>



#include "DynamicInputWidget.h"
#include "ISettingsWidget.h"


namespace AppSettings
{
	class ListSetting;
	namespace UI
	{
		class APP_SETTINGS_API ListSettingsWidget : public ISettingsWidget
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
			void saveSetting() override;

		private slots:
			void onValueChanged();

			void onAddButtonClicked();
			void onRemoveButtonClicked();


		private:
			void replaceAddButton();

			ListSetting* m_setting = nullptr;

			QLabel *m_nameLabel;
			QWidget* m_contentWidget;
			bool m_hasChanges = false;
			QGridLayout* m_contentGridLayout = nullptr;
			QPushButton* m_addButton = nullptr;
			
			class ListWidget
			{
			public:
				ListWidget(QVariant value, bool removeButtonEnabled);
				~ListWidget();

				DynamicInputWidget* value;
				QPushButton* removeButton = nullptr;
			};
			std::vector<ListWidget*> m_listWidgets;
		};
	}
}