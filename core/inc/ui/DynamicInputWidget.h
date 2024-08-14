#pragma once

#include "AppSettings_base.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QVariant>


namespace AppSettings
{
	namespace UI
	{
        /// <summary>
        /// This widget is used to edit a QVariant
        /// It automaticly changes the input field, depending on the type of the QVariant
        /// </summary>
        class APP_SETTINGS_EXPORT DynamicInputWidget : public QWidget {
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