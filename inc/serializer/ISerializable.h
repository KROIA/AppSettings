#pragma once
#include <QJsonObject>

namespace Settings
{
    class ISerializable
    {
        public:

            /**
             * \brief Reimplement this function in each derived class.
             * \return The Json object with all stored data for a later reinstantiation of this object
             * \details Save Example 1 see read() example 1
             * \code
             *         QJsonObject save() const override
             *         {
             *             // Combine the QJsonObject with the base object of this
             *             return combine(ISerializable::save(),
             *             QJsonObject
             *             {
             *                 // Add the properties of this object here
             *                 // Do not take the same keyvalues two times,
             *                 // also not the keys of the base class
             *                 {"name" , m_name.c_str()},
             *                 {"color", m_color.c_str()},
             *             });
             *         }
             * \endcode
             *
             * \details Save Example 2 see read() example 2
             * If you have instances of different ISerializable objects, than you can save them too.
             * \code
             *         QJsonObject save() const override
             *         {
             *             // Add this data to thisData
             *             QJsonObject thisData = combine(ISerializable::save(),
             *             QJsonObject
             *             {
             *                 // Add the properties of this object here
             *                 // Do not take the same keyvalues two times,
             *                 // also not the keys of the base class
             *                 {"name" , m_name.c_str()},
             *                 {"color", m_color.c_str()},
             *             });
             *
             *             // Get the QJsonObject of the member object
             *             thisData["myObject"] = myObject.save();
             *
             *             return thisData;
             *         }
             * \endcode
             *
             */
            virtual QJsonObject save() const = 0;

            /**
             * \brief Reads the saved data back into this object.
             *        Reimplement this function in each derived class.
             * \param reader to load the saved parameters back
             * \return true if the loading was a success, otherwise false
             * \details Read Example 1 see save() example 1
             *          The reader will contain the data for this object.
             *          You can only extract, what you saved before.
             * \code
             *          bool read(const QJsonObject &reader) override
             *          {
             *              return QJsonObject
             *              {
             *                  // Add the properties of this object here
             *                  // Do not take the same keyvalues two times,
             *                  // also not the keys of the base class
             *                  {"key" , m_parameter.first},
             *                  {"val",  m_parameter.second.toString()},
             *              };
             *          }
             * \endcode
             *
             *
             * \details Read Example 2 see save() example 2
             *          If you have instances of ISerializable objects, than you have to call the read function on them
             *          with the data for that object.
             * \code
             *         bool read(const QJsonObject &reader) override
             *         {
             *             bool success = true;
             *             // Read the value for the base class
             *             success = ISerializable::read(reader);
             *
             *             // Read the value for the member object
             *             success &= myObject.read(thisData["myObject"]);
             *
             *             // Read the values for this class
             *             success &= extract(reader,m_name, "name");
             *             success &= extract(reader,m_color,"color");
             *             return success;
             *         }
             * \endcode
             */
            virtual bool read(const QJsonObject &reader) = 0;
    };
}
