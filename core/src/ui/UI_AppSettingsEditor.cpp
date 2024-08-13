#include "ui/UI_AppSettingsEditor.h"
#include "SettingsGroup.h"
#include "Setting.h"

#include "ui/SettingsWidget.h"
#include "ui/MapSettingsWidget.h"


namespace AppSettings
{
	namespace UI
	{
		UI_AppSettingsEditor::UI_AppSettingsEditor(QWidget* parent)
			: QWidget(parent)
		{
			ui.setupUi(this);
			m_treeWidget = ui.group_treeWidget;
			m_scrollArea = ui.scrollArea;

			QWidget* central = new QWidget;
			QVBoxLayout* layout = new QVBoxLayout(central);
			central->setLayout(layout);
			m_scrollArea->setWidget(central);
			m_scrollArea->setWidgetResizable(true);
		}

		UI_AppSettingsEditor::~UI_AppSettingsEditor()
		{
		}


		void UI_AppSettingsEditor::onTreeElementClicked(QTreeWidgetItem* item, int column)
		{
			AS_UNUSED(column);
			for (size_t i = 0; i < m_treeItems.size(); ++i)
			{
				if (m_treeItems[i].widgetItem == item)
				{
					showSettingsGroup(m_treeItems[i].settingsGroup);
				}
			}
		}

		void UI_AppSettingsEditor::showEvent(QShowEvent* event)
		{
			applySettingGroupsToUI();
			QWidget::showEvent(event);
		}
		void UI_AppSettingsEditor::applySettingGroupsToUI()
		{
			m_treeWidget->clear();
			m_treeItems.clear();
			connect(m_treeWidget, &QTreeWidget::itemClicked, this, &UI_AppSettingsEditor::onTreeElementClicked);

			std::vector<SettingsGroup*>& groups = SettingsGroup::getRootGroups();
			for (size_t i = 0; i < groups.size(); ++i)
			{
				QTreeWidgetItem* settingGroupElement = new QTreeWidgetItem(m_treeWidget);
				settingGroupElement->setText(0, groups[i]->getName());
				TreeItem item;
				item.widgetItem = settingGroupElement;
				item.settingsGroup = groups[i];
				m_treeItems.push_back(item);
				
				addTreeWidgetRecursive(settingGroupElement, groups[i]->getGroups());
			}
		}
		void UI_AppSettingsEditor::showSettingsGroup(SettingsGroup* group)
		{
			if (!group)
				return;
			for (size_t i = 0; i < m_listViewWidgets.size(); ++i)
				delete m_listViewWidgets[i];
			m_listViewWidgets.clear();
			m_listViewWidgets.reserve(group->getSettingCount());

			const std::vector<ISetting*>& settings = group->getSettings();
			delete m_spacer;
			m_spacer = nullptr;

			for (size_t i = 0; i < settings.size(); ++i)
			{
				QWidget* w = createSettingsWidget(settings[i]);
				if (w)
				{
					m_listViewWidgets.push_back(w);
					m_scrollArea->widget()->layout()->addWidget(w);
				}
			}
			
			m_spacer = new QWidget(this);
			m_spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			m_scrollArea->widget()->layout()->addWidget(m_spacer);

		}
		QWidget* UI_AppSettingsEditor::createSettingsWidget(ISetting* setting)
		{
			if (Setting* s = dynamic_cast<Setting*>(setting))
			{
				UI::SettingsWidget* widget = new UI::SettingsWidget();
				widget->setSetting(s);
				return widget;
			}
			else if (MapSetting* ms = dynamic_cast<MapSetting*>(setting))
			{
				UI::MapSettingsWidget* widget = new UI::MapSettingsWidget();
				widget->setSetting(ms);
				return widget;
			}
			return nullptr;
		}

		void UI_AppSettingsEditor::addTreeWidgetRecursive(QTreeWidgetItem* parent, std::vector<SettingsGroup*> groups)
		{
			for (size_t i = 0; i < groups.size(); ++i)
			{
				QTreeWidgetItem* settingGroupElement = new QTreeWidgetItem(parent);
				settingGroupElement->setText(0, groups[i]->getName());
				TreeItem item;
				item.widgetItem = settingGroupElement;
				item.settingsGroup = groups[i];
				m_treeItems.push_back(item);
				addTreeWidgetRecursive(settingGroupElement, groups[i]->getGroups());
			}
		}
	}
}