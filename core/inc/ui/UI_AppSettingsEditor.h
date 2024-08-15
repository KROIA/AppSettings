#pragma once

#include "AppSettings_base.h"
#include <QWidget>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QScrollArea>
#include <unordered_map>
#include <memory>

#include "ISettingsWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UI_AppSettingsEditor; }
QT_END_NAMESPACE

namespace AppSettings
{
	class SettingsGroup;
	class ISetting;
	namespace UI
	{
		class APP_SETTINGS_EXPORT UI_AppSettingsEditor : public QWidget
		{
			Q_OBJECT

		public:
			UI_AppSettingsEditor(QWidget* parent = nullptr);
			~UI_AppSettingsEditor();

		private slots:
			void onTreeElementClicked(QTreeWidgetItem* item, int column);

			void on_save_pushButton_clicked();
			void on_cancel_pushButton_clicked();

		private:
			void showEvent(QShowEvent* event) override;
			void applySettingGroupsToUI();
			void showSettingsGroup(SettingsGroup* group);
			ISettingsWidget* createSettingsWidget(ISetting* setting);
			void addTreeWidgetRecursive(QTreeWidgetItem* parent, std::vector<SettingsGroup*> groups);


			Ui::UI_AppSettingsEditor *ui;
			QTreeWidget* m_treeWidget;
			QScrollArea* m_scrollArea;
			QWidget* m_spacer = nullptr;
			SettingsGroup* m_currentVisibleGroup = nullptr;

			struct TreeItem
			{
				QTreeWidgetItem* widgetItem;
				SettingsGroup* settingsGroup;
			};
			std::vector<TreeItem> m_treeItems;

			std::unordered_map<SettingsGroup*, std::vector<std::shared_ptr<ISettingsWidget>>> m_listViewWidgets;
		};
	}
}