#include "ui/DynamicInputWidget.h"

#include "CustomVariants/Selection.h"

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QCheckBox>
#include <QLabel>
#include <QCombobox>
#include <limits>

namespace AppSettings
{
	namespace UI
	{
        DynamicInputWidget::DynamicInputWidget(QWidget* parent) 
            : QWidget(parent) 
        {
            m_layout = new QVBoxLayout(this);
            m_layout->setMargin(0);
            setLayout(m_layout);
        }

        void DynamicInputWidget::setValue(const QVariant& value)
        {
            // Clear the m_layout before adding new input fields
            clearLayout();
            m_value = value;

            // Check the type of QVariant and create corresponding input field
            switch (value.type())
            {
                case QVariant::Int:
                {
                    QSpinBox* spinBox = new QSpinBox(this);
                    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [&] {emit valueChanged(); });
                    spinBox->setMinimum(std::numeric_limits<int>::min());
                    spinBox->setMaximum(std::numeric_limits<int>::max());

                    spinBox->setValue(value.toInt());
                    m_layout->addWidget(spinBox);
                    break;
                }
                case QVariant::Double:
				{
					QDoubleSpinBox* spinBox = new QDoubleSpinBox(this);
					connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [&] {emit valueChanged(); });
                    spinBox->setMinimum(std::numeric_limits<double>::min());
                    spinBox->setMaximum(std::numeric_limits<double>::max());

					spinBox->setValue(value.toDouble());
					m_layout->addWidget(spinBox);
					break;
				}
                case QVariant::String:
                {
                    QLineEdit* lineEdit = new QLineEdit(this);
					connect(lineEdit, &QLineEdit::textChanged, this, [&] {emit valueChanged(); });
					lineEdit->setText(value.toString());
					m_layout->addWidget(lineEdit);
					break;
				}
                case QVariant::Date:
				{
				    QDateEdit* dateEdit = new QDateEdit(this);
                    connect(dateEdit, &QDateEdit::dateChanged, this, [&] {emit valueChanged(); });
                    dateEdit->setDate(value.toDate());
                    m_layout->addWidget(dateEdit);
                    break;
                }
                case QVariant::Time:
                {
                    QTimeEdit* timeEdit = new QTimeEdit(this);
                    connect(timeEdit, &QTimeEdit::dateChanged, this, [&] {emit valueChanged(); });
                    timeEdit->setTime(value.toTime());
                    m_layout->addWidget(timeEdit);
                    break;
                }
                case QVariant::DateTime:
                {
                    QDateTimeEdit* dateTimeEdit = new QDateTimeEdit(this);
                    connect(dateTimeEdit, &QTimeEdit::timeChanged, this, [&] {emit valueChanged(); });
                    dateTimeEdit->setDateTime(value.toDateTime());
                    m_layout->addWidget(dateTimeEdit);
                    break;
                }
                case QVariant::Bool:
				{
                    QCheckBox* checkBox = new QCheckBox(this);
				    connect(checkBox, &QCheckBox::stateChanged, this, [&] {emit valueChanged(); });
				    checkBox->setChecked(value.toBool());
				    m_layout->addWidget(checkBox);
				    break;
				}
				default:
                {
                    if (value.canConvert<Selection>())
                    {
                        Selection selection = value.value<Selection>();
                        QComboBox* comboBox = new QComboBox(this);
                        comboBox->addItems(selection.getSelection());
                        comboBox->setCurrentIndex(selection.getSelectedIndex());
                        m_layout->addWidget(comboBox);
                    }
                    else
                    {
                        QLabel* unsupportedLabel = new QLabel("Unsupported type", this);
                        m_layout->addWidget(unsupportedLabel);
                    }
                    break;
                }
            }
        }

        QVariant DynamicInputWidget::getValue() const
        {
            // Get the value of the input field
            QVariant value;
            QWidget *inputWidget = m_layout->itemAt(0)->widget();
            if (QSpinBox* spinBox = qobject_cast<QSpinBox*>(inputWidget))
            {
                value = spinBox->value();
            }
            if (QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(inputWidget))
            {
                value = spinBox->value();
            }
            else if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(inputWidget))
            {
                value = lineEdit->text();
            }
            else if (QDateEdit* dateEdit = qobject_cast<QDateEdit*>(inputWidget))
            {
                value = dateEdit->date();
            }
            else if (QTimeEdit* timeEdit = qobject_cast<QTimeEdit*>(inputWidget))
            {
                value = timeEdit->time();
            }
            else if (QDateTimeEdit* dateTimeEdit = qobject_cast<QDateTimeEdit*>(inputWidget))
            {
                value = dateTimeEdit->dateTime();
            }
            else if (QCheckBox* checkBox = qobject_cast<QCheckBox*>(inputWidget))
            {
                value = checkBox->isChecked();
            }
            else if (QComboBox* comboBox = qobject_cast<QComboBox*>(inputWidget))
            {
                value = m_value;
                Selection selection = value.value<Selection>();
                selection.select(comboBox->currentIndex());
                value = QVariant::fromValue(selection);
            }
            return value;
        }

        void DynamicInputWidget::clearLayout()
        {
            while (QLayoutItem* item = m_layout->takeAt(0)) {
                delete item->widget();
                delete item;
            }
        }

	}
}