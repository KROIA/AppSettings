#pragma once


#include "SettingGroup.h"
#include "IJsonSerializable.h"

namespace Settings
{
    /**
     * \author  Alex Krieg
     * \version 00.00.01
     * \date    27.10.2022
     *
     * \brief This class is used to store multiple
     *        unique(by name) Settings
     *        This group also can be exportet/imported to/from Json
     *
     */
    class JsonSettingGroup  :   public SettingGroup, public IJsonSerializable
    {
         public:
            JsonSettingGroup();

            /**
             * \brief Constructor
             * \param name, name of this SettingGroup group
             */
            JsonSettingGroup(const QString &name);

            QJsonObject save() const override;              //!<\see ISerializable::save()
            bool read(const QJsonObject &reader) override;  //!<\see ISerializable::read()


            /**
             * @brief saveToFile
             * @param jsonFile path in which the data will be stored
             * @return true if the settings saved to json successfully
             */
            bool saveToFile(const QString &jsonFile);

            /**
             * @brief readFromFile
             * @param jsonFile path which the data will be loaded from
             * @return true if the loading was successful
             */
            bool readFromFile(const QString &jsonFile);



        protected:
            Setting *createSettingInstance() override;


    };
}
