#include "JsonSettingGroup.h"
#include "JsonSetting.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

namespace Settings
{
JsonSettingGroup::JsonSettingGroup()
    : SettingGroup()
    , IJsonSerializable()
{

}


JsonSettingGroup::JsonSettingGroup(const QString &name)
    : SettingGroup(name)
    , IJsonSerializable()
{

}


QJsonObject JsonSettingGroup::save() const
{
    QJsonObject writer;
    writer["name"] = m_name;


    size_t sCount = 0;
    size_t cCount = 0;
    for(size_t i=0; i<m_settings.size(); ++i)
    {
        JsonSetting *jSetting = dynamic_cast<JsonSetting*>(m_settings[i]);
        if(jSetting)
        {
            writer["S"+QString::number(i)] = jSetting->save();
            ++sCount;
        }else
        {
            qDebug() << __PRETTY_FUNCTION__ << " Can't save setting: "<<getName()<<"."<<m_settings[i] << " object is not of type JsonSetting";
        }
    }
    for(size_t i=0; i<m_settingGroups.size(); ++i)
    {
        JsonSettingGroup *jGroup = dynamic_cast<JsonSettingGroup*>(m_settingGroups[i]);

        if(jGroup)
        {
            writer["C"+QString::number(i)] = jGroup->save();
            ++cCount;
        }else
        {
            qDebug() << __PRETTY_FUNCTION__ << " Can't save group: "<<getName()<<"."<<m_settingGroups[i]->getName() << " object is not of type JsonSettingGroup";
        }
    }
    writer["sc"] = (int)sCount;
    writer["cc"] = (int)cCount;
    return writer;
}
bool JsonSettingGroup::read(const QJsonObject &reader)
{
    bool success = true;
    m_name = reader["name"].toString("Settings");
    int settingsCount = reader["sc"].toInt();
    int childGroupsCount = reader["cc"].toInt();
    if(m_isEmpty)
    {
        // Generating mode
        for(int i=0; i<settingsCount; ++i)
        {
            QJsonObject settingData = reader["S"+QString::number(i)].toObject();
            JsonSetting newSetting;

            success &= newSetting.read(settingData);
            success &= add(newSetting.getName(), newSetting.getValue()) != nullptr;
        }
        for(int i=0; i<childGroupsCount; ++i)
        {
            QJsonObject groupData = reader["C"+QString::number(i)].toObject();
            JsonSettingGroup *newGroup = new JsonSettingGroup();
            newGroup->read(groupData);

            success &= newGroup->read(groupData);

            if(!SettingGroup::add(newGroup))
            {

            }
        }
    }
    else
    {
        // Overwrite mode
        for(int i=0; i<settingsCount; ++i)
        {
            QJsonObject settingData = reader["S"+QString::number(i)].toObject();
            JsonSetting newSetting;

            success &= newSetting.read(settingData);
            Setting *setting = getSetting(newSetting.getName());
            if(setting)
                setting->setValue(newSetting.getValue());
            else
                success = false;
        }
    }
    return success;
}

bool JsonSettingGroup::saveToFile(const QString &jsonFile)
{
    QJsonDocument document;
    document.setObject(save());
    QByteArray bytes = document.toJson( QJsonDocument::Indented );
    QFile file(jsonFile);
    if( file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
    {
        QTextStream iStream( &file );
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        iStream.setEncoding(QStringConverter::Encoding::Utf8);
#else
        iStream.setCodec("UTF-8");
#endif
        iStream << bytes;
        file.close();
        return true;
    }
    SETTINGS_WARNING_PRETTY << "SettingGroup: "<<getName()<<" konnte nicht nach \""<<jsonFile<<"\" gespeichert werden.";
    return false;
}
bool JsonSettingGroup::readFromFile(const QString &jsonFile)
{
    QFile file(jsonFile);
    if(!file.exists())
    {
        SETTINGS_WARNING_PRETTY << "SettingGroup konnte nicht von \""<<jsonFile<<"\" geladen werden. Kann die angegebene Datei nicht finden.";
        return false;
    }
    if( file.open(QIODevice::ReadOnly ) )
    {
        QByteArray bytes = file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
        if( jsonError.error != QJsonParseError::NoError )
        {
            SETTINGS_WARNING_PRETTY << "SettingGroup konnte nicht von \""<<jsonFile<<"\" geladen werden.";
            return false;
        }
        if(document.isObject())
        {
            QJsonObject obj = document.object();
            return read(obj);
        }
    }
    SETTINGS_WARNING_PRETTY << "SettingGroup konnte nicht von \""<<jsonFile<<"\" geladen werden.";
    return false;
}


Setting *JsonSettingGroup::createSettingInstance()
{
    return new JsonSetting();
}
}
