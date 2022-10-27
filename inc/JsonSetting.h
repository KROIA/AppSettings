#pragma once

#include "IJsonSerializable.h"
#include "Setting.h"

namespace Settings
{

    /**
     * \author  Alex Krieg
     * \version 00.00.01
     * \date    27.10.2022
     *
     * \brief This class is used to store a name with a value.
     *        It can also be exportet/imported to/from Json
     *
     */
    class JsonSetting: public Setting, public IJsonSerializable
    {
        public:
            /**
             * \brief Constructor
             */
            JsonSetting();

            /**
             * \brief Copy constructor
             * \param other, The other object from which will be copied
             */
            JsonSetting(const Setting &other);

            /**
             * \brief Constructor
             * \param name,  The name of the setting
             * \param value, The value of the parameter, can by any QVariant compatible type
             */
            JsonSetting(const QString &name, const QVariant value);

            /**
             * \brief Constructor
             * \param setting,  a pair of name and value
             */
            JsonSetting(const std::pair<QString,QVariant> &setting);

            QJsonObject save() const override;              //!<\see ISerializable::save()
            bool read(const QJsonObject &reader) override;  //!<\see ISerializable::read()
    };
}
