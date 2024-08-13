#pragma once

#include "AppSettings_base.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QVariant>


namespace AppSettings
{
	namespace UI
	{

        class DynamicInputWidget : public QWidget {
            Q_OBJECT

        public:
            DynamicInputWidget(QWidget* parent = nullptr);

            void setValue(const QVariant& value);
            QVariant getValue() const;

        signals:
            void valueChanged();
        private:
            void clearLayout();

            QVBoxLayout* m_layout;
            QVariant m_value;
        };
	}
}