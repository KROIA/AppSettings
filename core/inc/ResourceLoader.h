#pragma once

#include "AppSettings_base.h"
#include <QIcon>

namespace AppSettings
{
	class ResourceLoader
	{
	public:
		ResourceLoader();

		static ResourceLoader& instance();

		static const QIcon& getIcon(const QString& name);
	private:
		void loadIcon(const QString& name);
		std::map<QString, QIcon> m_icons;

	};
}