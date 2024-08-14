#include "ui/ListSettingsWidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QVariant>
#include <QFrame>

#include "ListSetting.h"

namespace AppSettings
{
	namespace UI
	{
		ListSettingsWidget::ListSettingsWidget(QWidget* parent)
			: QWidget(parent)
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

		ListSettingsWidget::~ListSettingsWidget()
		{

		}

		void ListSettingsWidget::setSetting(ListSetting* setting)
		{
			m_setting = setting;
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				delete m_mapWidgets[i];
			}
			m_mapWidgets.clear();
			delete m_contentWidget->layout();
			QGridLayout* gridLayout = new QGridLayout(this);
			m_contentWidget->setLayout(gridLayout);

			if (m_setting)
			{
				m_nameLabel->setText(m_setting->getName());
				const std::map<QVariant, QVariant>& map = m_setting->getMap();
				for (const auto& it : map)
				{
					MapWidget* pair = new MapWidget(it.first, it.second);
					connect(pair->key, &DynamicInputWidget::valueChanged, this, &ListSettingsWidget::onKeyChanged);
					connect(pair->value, &DynamicInputWidget::valueChanged, this, &ListSettingsWidget::onValueChanged);

					
					gridLayout->addWidget(pair->key, m_mapWidgets.size(), 0);
					gridLayout->addWidget(pair->value, m_mapWidgets.size(), 1);
					m_mapWidgets.push_back(pair);

				}

				
			}
			else
			{
				m_nameLabel->setText("NULL");
			}
		}

		void ListSettingsWidget::saveSetting()
		{
			if (!m_setting)
				return;
			//m_setting->setValue(m_inputWidget->getValue());
			std::map<QVariant, QVariant> map;
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				map[m_mapWidgets[i]->key->getValue()] = m_mapWidgets[i]->value->getValue();
			}
			m_setting->setMap(map);
		}


		void ListSettingsWidget::onKeyChanged()
		{
			if (!m_setting)
				return;

			std::map<QVariant, QVariant> map;
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				map[m_mapWidgets[i]->key->getValue()] = m_mapWidgets[i]->value->getValue();
			}
			m_hasChanges = (m_setting->getMap() != map);
		}
		void ListSettingsWidget::onValueChanged()
		{
			if (!m_setting)
				return;

			std::map<QVariant, QVariant> map;
			for (size_t i = 0; i < m_mapWidgets.size(); ++i)
			{
				map[m_mapWidgets[i]->key->getValue()] = m_mapWidgets[i]->value->getValue();
			}
			m_hasChanges = (m_setting->getMap() != map);
		}




		ListSettingsWidget::MapWidget::MapWidget(QVariant key, QVariant value)
		{
			this->key = new DynamicInputWidget();
			this->value = new DynamicInputWidget();

			this->key->setValue(key);
			this->value->setValue(value);
		}
		ListSettingsWidget::MapWidget::~MapWidget()
		{
			delete key;
			delete value;
		}
	}
}