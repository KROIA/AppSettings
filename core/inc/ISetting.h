#pragma once

#include "AppSettings_base.h"
#include "IJsonSerializable.h"
#include <QString>
#include <QDebug>
#include <QObject>
#include <iostream>

namespace AppSettings
{
	class ISetting : public QObject, public IJsonSerializable
	{
        Q_OBJECT
	public:
        virtual void setName(const QString& name) = 0;
		virtual QString getName() const = 0;
		virtual QString toString() const = 0;




        /// <summary>
        /// Prints the setting to the QT-debug console in the same form as this->toString()
        /// Usage:
        ///     Setting mySetting();
        ///     qDebug() << mySetting;
        /// </summary>
        /// <param name="debug"></param>
        /// <param name="setting"></param>
        /// <returns></returns>
        friend QDebug operator<<(QDebug debug, const ISetting& setting)
        {
            debug.nospace() << setting.toString();
            return debug;
        }

        /// <summary>
        /// Prints the setting to the std::ostream in the same form as this->toString()
        /// Usage:
        /// Setting mySetting();
        /// std::cout << mySetting;
        /// </summary>
        /// <param name="stream"></param>
        /// <param name="setting"></param>
        /// <returns></returns>
        friend std::ostream& operator<<(std::ostream& stream, const ISetting& setting)
        {
            stream << setting.toString().toStdString();
            return stream;
        }

    signals:
        void valueChanged();
        void nameChanged();
		
	};
}