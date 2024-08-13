#pragma once

#include "AppSettings_base.h"
#include <QWidget>
#include "ui_UI_AppSettingsEditor.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QScrollArea>

namespace AppSettings
{
	class SettingsGroup;
	class ISetting;
	namespace UI
	{
		class UI_AppSettingsEditor : public QWidget
		{
			Q_OBJECT

		public:
			UI_AppSettingsEditor(QWidget* parent = nullptr);
			~UI_AppSettingsEditor();

		private slots:
			void onTreeElementClicked(QTreeWidgetItem* item, int column);

		private:
			void showEvent(QShowEvent* event) override;
			void applySettingGroupsToUI();
			void showSettingsGroup(SettingsGroup* group);
			QWidget* createSettingsWidget(ISetting* setting);
			void addTreeWidgetRecursive(QTreeWidgetItem* parent, std::vector<SettingsGroup*> groups);


			Ui::UI_AppSettingsEditor ui;
			QTreeWidget* m_treeWidget;
			QScrollArea* m_scrollArea;
			QWidget* m_spacer = nullptr;

			struct TreeItem
			{
				QTreeWidgetItem* widgetItem;
				SettingsGroup* settingsGroup;
			};
			std::vector<TreeItem> m_treeItems;

			std::vector<QWidget*> m_listViewWidgets;
		};
	}
}