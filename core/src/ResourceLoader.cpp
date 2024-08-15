#include "ResourceLoader.h"

namespace AppSettings
{
	

	ResourceLoader::ResourceLoader()
	{
		Q_INIT_RESOURCE(AppSettingsIcons); // Init the resource file

	}

	ResourceLoader& ResourceLoader::instance()
	{
		static ResourceLoader instance;
		return instance;
	}

	const QIcon& ResourceLoader::getIcon(const QString& name)
	{
		ResourceLoader &instance = ResourceLoader::instance();
		if(instance.m_icons.find(name) == instance.m_icons.end())
		{
			instance.loadIcon(name);
		}
		return instance.m_icons.at(name);
	}
	void ResourceLoader::loadIcon(const QString& name)
	{
		m_icons[name] = QIcon(QString(":/AppSettingsIcons/icons/%1").arg(name));
	}
}