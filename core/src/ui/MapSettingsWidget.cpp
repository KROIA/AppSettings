#include "ui/MapSettingsWidget.h"
#include <QVBoxLayout>
#include <QVariant>
#include <QFrame>

#include "ui/DynamicInputWidget.h"
#include "MapSetting.h"
#include "ResourceLoader.h"

namespace AppSettings
{
	namespace UI
	{
		MapSettingsWidget::MapSettingsWidget(QWidget* parent)
			: ISettingsWidget(parent)
		{
			
			//QHBoxLayout *hLayout = new QHBoxLayout(this);
			m_contentWidget = new QWidget(this);
			
			
			QVBoxLayout* layout1 = new QVBoxLayout(this);
			layout1->setMargin(0);
			setLayout(layout1);
			QFrame* frame = new QFrame(this);
			frame->setFrameShape(QFrame::Shape::Box);
			frame->setLineWidth(1);

			QVBoxLayout* vLayout = new QVBoxLayout(frame);
			frame->setLayout(vLayout);
			//frame->setMinimumSize(QSize(100, 100));
			layout1->addWidget(frame);
			m_nameLabel = new QLabel(this);
			vLayout->addWidget(m_nameLabel);
			vLayout->addWidget(m_contentWidget);
			
			
		}

		MapSettingsWidget::~MapSettingsWidget()
		{

		}

		void MapSettingsWidget::setSetting(MapSetting* setting)
		{
			m_setting = setting;
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				delete m_mapWidgets[i];
			}
			m_mapWidgets.clear();
			//QLayout *oldLayout =  m_contentWidget->layout();
			QGridLayout* gridLayout = new QGridLayout(m_contentWidget);
			m_contentGridLayout = gridLayout;
			m_contentWidget->setLayout(gridLayout);
			//oldLayout->deleteLater();
			

			if (m_setting)
			{
				bool removeButtonEnabled = m_setting->isRemoveButtonEnabled();
				bool addButtonEnabled = m_setting->isAddButtonEnabled();
				m_nameLabel->setText(m_setting->getName());
				const std::vector<std::pair<QVariant, QVariant>>& map = m_setting->getMap();
				for (const auto& it : map)
				{
					MapWidget* pair = new MapWidget(it.first, it.second, removeButtonEnabled);
					connect(pair->key, &DynamicInputWidget::valueChanged, this, &MapSettingsWidget::onKeyChanged);
					connect(pair->value, &DynamicInputWidget::valueChanged, this, &MapSettingsWidget::onValueChanged);

					
					gridLayout->addWidget(pair->key, m_mapWidgets.size(), 0);
					gridLayout->addWidget(pair->value, m_mapWidgets.size(), 1);
					if (pair->removeButton)
					{
						connect(pair->removeButton, &QPushButton::clicked, this, &MapSettingsWidget::onRemoveButtonClicked);
						gridLayout->addWidget(pair->removeButton, m_mapWidgets.size(), 2);
					}
					m_mapWidgets.push_back(pair);

				}

				if (addButtonEnabled)
				{
					if (!m_addButton)
					{
						m_addButton = new QPushButton("");
						m_addButton->setIcon(ResourceLoader::getIcon("add.png"));
						// Remove the default button background and border using stylesheets
						m_addButton->setStyleSheet("QPushButton {"
							"border: none;"        // No border
							"background: none;"    // No background
							"padding: 0;"          // No padding
							"}");
						int buttonSize = 20;
						m_addButton->setIconSize(QSize(buttonSize, buttonSize));
					}
					connect(m_addButton, &QPushButton::clicked, this, &MapSettingsWidget::onAddButtonClicked);
					replaceAddButton();
				}
			}
			else
			{
				m_nameLabel->setText("NULL");
			}
		}

		void MapSettingsWidget::saveSetting()
		{
			if (!m_setting)
				return;
			//m_setting->setValue(m_inputWidget->getValue());
			std::vector<std::pair<QVariant, QVariant>> map;
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				map.push_back(std::make_pair(m_mapWidgets[i]->key->getValue(), m_mapWidgets[i]->value->getValue()));
			}
			m_setting->setMap(map);
		}


		void MapSettingsWidget::onKeyChanged()
		{
			if (!m_setting)
				return;

			std::vector<std::pair<QVariant, QVariant>> map;
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				map.push_back(std::make_pair(m_mapWidgets[i]->key->getValue(), m_mapWidgets[i]->value->getValue()));
			}
			m_hasChanges = (m_setting->getMap() != map);
		}
		void MapSettingsWidget::onValueChanged()
		{
			if (!m_setting)
				return;

			std::vector<std::pair<QVariant, QVariant>> map;
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				map.push_back(std::make_pair(m_mapWidgets[i]->key->getValue(), m_mapWidgets[i]->value->getValue()));
			}
			m_hasChanges = (m_setting->getMap() != map);
		}
		
		void MapSettingsWidget::onAddButtonClicked()
		{
			if (!m_setting)
				return;
			MapWidget* pair = new MapWidget(QVariant(""), QVariant(""), m_setting->isRemoveButtonEnabled());
			connect(pair->key, &DynamicInputWidget::valueChanged, this, &MapSettingsWidget::onKeyChanged);
			connect(pair->value, &DynamicInputWidget::valueChanged, this, &MapSettingsWidget::onValueChanged);


			m_contentGridLayout->addWidget(pair->key, m_mapWidgets.size(), 0);
			m_contentGridLayout->addWidget(pair->value, m_mapWidgets.size(), 1);
			if (pair->removeButton)
			{
				connect(pair->removeButton, &QPushButton::clicked, this, &MapSettingsWidget::onRemoveButtonClicked);
				m_contentGridLayout->addWidget(pair->removeButton, m_mapWidgets.size(), 2);
			}
			m_mapWidgets.push_back(pair);
			replaceAddButton();
		}
		void MapSettingsWidget::onRemoveButtonClicked()
		{
			QPushButton *sender = qobject_cast<QPushButton*>(QObject::sender());
			for(size_t i=0; i<m_mapWidgets.size(); ++i)
			{
				MapWidget* pair = m_mapWidgets[i];
				m_contentGridLayout->removeWidget(pair->key);
				m_contentGridLayout->removeWidget(pair->value);
				m_contentGridLayout->removeWidget(pair->removeButton);
				if (m_mapWidgets[i]->removeButton == sender)
				{
					
					m_mapWidgets.erase(m_mapWidgets.begin() + i);
					delete pair;
					--i;
				}
			}
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				m_contentGridLayout->addWidget(m_mapWidgets[i]->key, i, 0);
				m_contentGridLayout->addWidget(m_mapWidgets[i]->value, i, 1);
				if (m_mapWidgets[i]->removeButton)
				{
					m_contentGridLayout->addWidget(m_mapWidgets[i]->removeButton, i, 2);
				}
			}
			replaceAddButton();			
		}
		void MapSettingsWidget::replaceAddButton()
		{
			m_contentGridLayout->addWidget(m_addButton, m_mapWidgets.size(), 0,1, m_contentGridLayout->columnCount());
		}



		MapSettingsWidget::MapWidget::MapWidget(QVariant key, QVariant value, bool removeButtonEnabled)
		{
			this->key = new DynamicInputWidget();
			this->value = new DynamicInputWidget();
			if (removeButtonEnabled)
			{
				this->removeButton = new QPushButton("");
				//const QIcon &icon = ResourceLoader::getIcon("delete.png");
				this->removeButton->setIcon(ResourceLoader::getIcon("delete.png"));
				// Remove the default button background and border using stylesheets
				this->removeButton->setStyleSheet("QPushButton {"
					"border: none;"        // No border
					"background: none;"    // No background
					"padding: 0;"          // No padding
					"}");
				int buttonSize = 20;
				this->removeButton->setIconSize(QSize(buttonSize, buttonSize));
			}
			else
				this->removeButton = nullptr;

			this->key->setValue(key);
			this->value->setValue(value);
		}
		MapSettingsWidget::MapWidget::~MapWidget()
		{
			delete key;
			delete value;
			delete removeButton;
		}
	}
}