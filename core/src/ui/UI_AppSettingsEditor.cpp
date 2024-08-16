#include "ui/UI_AppSettingsEditor.h"
#include "ui_UI_AppSettingsEditor.h"
#include "SettingsGroup.h"
#include "Setting.h"
#include "MapSetting.h"
#include "ListSetting.h"

#include "ui/SettingsWidget.h"
#include "ui/MapSettingsWidget.h"
#include "ui/ListSettingsWidget.h"

#include "ApplicationSettings.h"

#include "Utilities.h"

namespace AppSettings
{
	namespace UI
	{
		UI_AppSettingsEditor::UI_AppSettingsEditor(QWidget* parent)
			: QWidget(parent)
			, ui(new Ui::UI_AppSettingsEditor)
		{
			ui->setupUi(this);
			m_treeWidget = ui->group_treeWidget;
			m_scrollArea = ui->scrollArea;

			QWidget* central = new QWidget;
			QVBoxLayout* layout = new QVBoxLayout(central);
			central->setLayout(layout);
			m_scrollArea->setWidget(central);
			m_scrollArea->setWidgetResizable(true);
		}

		UI_AppSettingsEditor::~UI_AppSettingsEditor()
		{
		}


		void UI_AppSettingsEditor::onTreeWidgetSelecionChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
		{
			AS_UNUSED(previous);
			for (size_t i = 0; i < m_treeItems.size(); ++i)
			{
				if (m_treeItems[i].widgetItem == current)
				{
					showSettingsGroup(m_treeItems[i].settingsGroup);
					return;
				}
			}
		}

		void UI_AppSettingsEditor::closeWindow()
		{
			hide();
			m_listViewWidgets.clear();
		}

		void UI_AppSettingsEditor::showEvent(QShowEvent* event)
		{
			QWidget::showEvent(event);
			applySettingGroupsToUI();
			
		}
		void UI_AppSettingsEditor::applySettingGroupsToUI()
		{
			m_treeWidget->clear();
			m_treeItems.clear();
			//connect(m_treeWidget, &QTreeWidget::itemClicked, this, &UI_AppSettingsEditor::onTreeElementClicked);
			connect(m_treeWidget, &QTreeWidget::currentItemChanged, this, &UI_AppSettingsEditor::onTreeWidgetSelecionChanged);

			m_listViewWidgets.clear();

			std::vector<SettingsGroup*>& groups = SettingsGroup::getRootGroups();
			for (size_t i = 0; i < groups.size(); ++i)
			{
				QTreeWidgetItem* settingGroupElement = new QTreeWidgetItem(m_treeWidget);
				settingGroupElement->setText(0, groups[i]->getName());
				settingGroupElement->setExpanded(true);
				TreeItem item;
				item.widgetItem = settingGroupElement;
				item.settingsGroup = groups[i];
				m_treeItems.push_back(item);
				
				addTreeWidgetRecursive(settingGroupElement, groups[i]->getGroups());
			}
			if (m_treeItems.size() > 0)
			{
				m_treeItems[0].widgetItem->setSelected(true);
				showSettingsGroup(m_treeItems[0].settingsGroup);
			}

		}
		void UI_AppSettingsEditor::showSettingsGroup(SettingsGroup* group)
		{
			if (!group)
				return;
		//	if (m_currentVisibleGroup == group)
		//		return;
			//for (size_t i = 0; i < m_listViewWidgets.size(); ++i)
			//	delete m_listViewWidgets[i];
			if (m_currentVisibleGroup)
			{
				// Remove all widgets from the layout
				const auto& it = m_listViewWidgets.find(m_currentVisibleGroup);
				if (it != m_listViewWidgets.end())
				{
					const std::vector<std::shared_ptr<ISettingsWidget>>& widgets = it->second;
					for (size_t i = 0; i < widgets.size(); ++i)
					{
						m_scrollArea->widget()->layout()->removeWidget(widgets[i].get());
						widgets[i].get()->hide();
					}
				}
			}
			
			
			//logger().logInfo("Selected: "+group->getName().toStdString());
			const std::vector<ISetting*>& settings = group->getSettings();
			delete m_spacer;
			m_spacer = nullptr;

			auto &widgets = m_listViewWidgets.find(group);
			if (widgets != m_listViewWidgets.end())
			{
				for (size_t i = 0; i < widgets->second.size(); ++i)
				{
					m_scrollArea->widget()->layout()->addWidget(widgets->second[i].get());
					widgets->second[i].get()->show();
				}
			}
			else
			{
				std::vector<std::shared_ptr<ISettingsWidget>> newWidgets;
				for (size_t i = 0; i < settings.size(); ++i)
				{
					ISettingsWidget* w = createSettingsWidget(settings[i]);
					if (w)
					{
						newWidgets.push_back(std::shared_ptr<ISettingsWidget>(w));
						m_scrollArea->widget()->layout()->addWidget(w);
					}
				}
				m_listViewWidgets[group] = newWidgets;
			}
			
			m_spacer = new QWidget(this);
			m_spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			m_scrollArea->widget()->layout()->addWidget(m_spacer);
			m_currentVisibleGroup = group;
		}
		ISettingsWidget* UI_AppSettingsEditor::createSettingsWidget(ISetting* setting)
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
			}else if(ListSetting *ls = dynamic_cast<ListSetting*>(setting))
			{
				UI::ListSettingsWidget* widget = new UI::ListSettingsWidget();
				widget->setSetting(ls);
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
				settingGroupElement->setExpanded(true);
				TreeItem item;
				item.widgetItem = settingGroupElement;
				item.settingsGroup = groups[i];
				m_treeItems.push_back(item);
				addTreeWidgetRecursive(settingGroupElement, groups[i]->getGroups());
			}
		}


		void UI_AppSettingsEditor::on_save_pushButton_clicked()
		{
			for(auto& it : m_listViewWidgets)
			{
				for (size_t i = 0; i < it.second.size(); ++i)
				{
					it.second[i]->saveSetting();
				}
			}
			ApplicationSettings::saveAll();
			closeWindow();
		}
		void UI_AppSettingsEditor::on_cancel_pushButton_clicked()
		{
			closeWindow();
		}
	}
}