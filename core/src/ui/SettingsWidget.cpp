#include "ui/SettingsWidget.h"
#include "ui/DynamicInputWidget.h"

namespace AppSettings
{
	namespace UI
	{
		SettingsWidget::SettingsWidget(QWidget* parent)
			: ISettingsWidget(parent)
		{
			m_layout = new QHBoxLayout(this);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
			m_layout->setMargin(0);
#else
			m_layout->setContentsMargins(0, 0, 0, 0);
#endif
			setLayout(m_layout);
			m_inputWidget = new DynamicInputWidget(this);
			m_nameLabel = new QLabel(this);
			m_layout->addWidget(m_nameLabel);
			m_layout->addWidget(m_inputWidget);

			connect(m_inputWidget, &DynamicInputWidget::valueChanged, this, &SettingsWidget::valueChanged);
		}

		SettingsWidget::~SettingsWidget()
		{

		}

		void SettingsWidget::setSetting(Setting* setting)
		{
			m_setting = setting;
			if (m_setting)
			{
				m_nameLabel->setText(m_setting->getName());
				m_inputWidget->setValue(m_setting->getValue());
			}
			else
			{
				m_nameLabel->setText("NULL");
				m_inputWidget->setValue(QVariant());
			}
		}

		void SettingsWidget::saveSetting() 
		{
			if (!m_setting)
				return;
			m_setting->setValue(m_inputWidget->getValue());
		}

		void SettingsWidget::onValueChanged()
		{
			if (!m_setting)
				return;
			m_hasChanges = (m_setting->getValue() != m_inputWidget->getValue());
		}
	}
}