#include "ui/ListSettingsWidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QVariant>
#include <QFrame>

#include "ListSetting.h"
#include "ResourceLoader.h"

namespace AppSettings
{
	namespace UI
	{
		ListSettingsWidget::ListSettingsWidget(QWidget* parent)
			: ISettingsWidget(parent)
		{

			//QHBoxLayout *hLayout = new QHBoxLayout(this);
			m_contentWidget = new QWidget(this);


			QVBoxLayout* layout1 = new QVBoxLayout(this);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
			layout1->setMargin(0);
#else
			layout1->setContentsMargins(0, 0, 0, 0);
#endif
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
			for (size_t i = 0; i < m_listWidgets.size(); ++i)
			{
				delete m_listWidgets[i];
			}
			m_listWidgets.clear();
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
				const std::vector<QVariant>& list = m_setting->getData();
				for (const auto& it : list)
				{
					ListWidget* listWidget = new ListWidget(it, removeButtonEnabled);
					connect(listWidget->value, &DynamicInputWidget::valueChanged, this, &ListSettingsWidget::onValueChanged);


					gridLayout->addWidget(listWidget->value, m_listWidgets.size(), 0);
					if (listWidget->removeButton)
					{
						connect(listWidget->removeButton, &QPushButton::clicked, this, &ListSettingsWidget::onRemoveButtonClicked);
						gridLayout->addWidget(listWidget->removeButton, m_listWidgets.size(), 1);
					}
					m_listWidgets.push_back(listWidget);

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
					connect(m_addButton, &QPushButton::clicked, this, &ListSettingsWidget::onAddButtonClicked);
					replaceAddButton();
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
			std::vector<QVariant> data;
			for (size_t i = 0; i < m_listWidgets.size(); ++i)
			{
				data.push_back(m_listWidgets[i]->value->getValue());
			}
			m_setting->setData(data);
		}


		void ListSettingsWidget::onValueChanged()
		{
			if (!m_setting)
				return;

			std::vector<QVariant> data;
			for (size_t i = 0; i < m_listWidgets.size(); ++i)
			{
				data.push_back(m_listWidgets[i]->value->getValue());
			}
			m_hasChanges = (m_setting->getData() != data);
		}

		void ListSettingsWidget::onAddButtonClicked()
		{
			if (!m_setting)
				return;
			ListWidget* widget = new ListWidget(QVariant(""), m_setting->isRemoveButtonEnabled());
			connect(widget->value, &DynamicInputWidget::valueChanged, this, &ListSettingsWidget::onValueChanged);

			m_contentGridLayout->addWidget(widget->value, m_listWidgets.size(), 0);
			if (widget->removeButton)
			{
				connect(widget->removeButton, &QPushButton::clicked, this, &ListSettingsWidget::onRemoveButtonClicked);
				m_contentGridLayout->addWidget(widget->removeButton, m_listWidgets.size(), 1);
			}
			m_listWidgets.push_back(widget);
			replaceAddButton();
		}
		void ListSettingsWidget::onRemoveButtonClicked()
		{
			QPushButton* sender = qobject_cast<QPushButton*>(QObject::sender());
			for (size_t i = 0; i < m_listWidgets.size(); ++i)
			{
				ListWidget* widget = m_listWidgets[i];
				m_contentGridLayout->removeWidget(widget->value);
				m_contentGridLayout->removeWidget(widget->removeButton);
				if (m_listWidgets[i]->removeButton == sender)
				{
					
					m_listWidgets.erase(m_listWidgets.begin() + i);
					delete widget;
					--i;
				}
			}
			for (size_t i = 0; i < m_listWidgets.size(); ++i)
			{
				m_contentGridLayout->addWidget(m_listWidgets[i]->value, i, 0);
				if (m_listWidgets[i]->removeButton)
				{
					m_contentGridLayout->addWidget(m_listWidgets[i]->removeButton, i, 1);
				}
			}
			replaceAddButton();
		}

		void ListSettingsWidget::replaceAddButton()
		{
			m_contentGridLayout->addWidget(m_addButton, m_listWidgets.size(), 0, 1, m_contentGridLayout->columnCount());
		}


		ListSettingsWidget::ListWidget::ListWidget(QVariant value, bool removeButtonEnabled)
		{
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

			this->value->setValue(value);
		}
		ListSettingsWidget::ListWidget::~ListWidget()
		{
			delete value;
			delete removeButton;
		}
	}
}