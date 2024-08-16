#ifdef QT_ENABLED
#include <QApplication>
#endif
#include <iostream>
#include <AppSettings.h>
#include <Logger.h>


class Group0 : public AppSettings::SettingsGroup
{
public:
	Group0()
		: SettingsGroup("Group0")
	{
		addSetting(m_testSetting0);
		addSetting(m_testSetting1);
		addSetting(m_testSetting2);
		addSetting(m_testSetting3);
		addSetting(m_testSetting4);
		addSetting(m_listSetting);
		addSetting(m_mapSetting);
		m_listSetting.setAddButtonEnabled(false);
		m_listSetting.setRemoveButtonEnabled(false);
	}

	AppSettings::Setting m_testSetting0 = AppSettings::Setting("Value0", 10);
	AppSettings::Setting m_testSetting1 = AppSettings::Setting("Value1", 11);
	AppSettings::Setting m_testSetting2 = AppSettings::Setting("Value2", 12);
	AppSettings::Setting m_testSetting3 = AppSettings::Setting("Value3", 13);
	AppSettings::Setting m_testSetting4 = AppSettings::Setting("Selection2", QVariant::fromValue(AppSettings::Selection({ "Sel1","Sel2","Sel3" },2)));
	AppSettings::ListSetting m_listSetting = AppSettings::ListSetting("ListSetting", { "Name1", "Name2", "Name3", 5 });
	AppSettings::MapSetting m_mapSetting = AppSettings::MapSetting("MapSetting", { {"Name1", 1}, {"Name2", 2}, {"Name3", 3}, 
		{"Name4", 4}, {5.5, "Value"}, { "Selection", QVariant::fromValue(AppSettings::Selection({"Sel1","Sel2","Sel3"})) } });
};
class Group1 : public AppSettings::SettingsGroup
{
public:
	Group1()
		: SettingsGroup("Group1")
	{
		addSetting(m_testSetting0);
		addSetting(m_testSetting1);
		addSetting(m_testSetting2);
		addSetting(m_testSetting3);
	}

	AppSettings::Setting m_testSetting0 = AppSettings::Setting("TestValue0", 0);
	AppSettings::Setting m_testSetting1 = AppSettings::Setting("TestValue1", 1);
	AppSettings::Setting m_testSetting2 = AppSettings::Setting("TestValue2", 2);
	AppSettings::Setting m_testSetting3 = AppSettings::Setting("TestValue3", 3);
};

class Group2 : public AppSettings::SettingsGroup
{
public:
	Group2()
		: SettingsGroup("Group2")
	{
		addSetting(m_testSetting0);
		addSetting(m_testSetting1);
		addSetting(m_testSetting2);
		addSetting(m_testSetting3);
		addSetting(m_testSetting4);
		addGroup(m_group0);
		addGroup(m_group1);
	}

	AppSettings::Setting m_testSetting0 = AppSettings::Setting("Name1", "Peter");
	AppSettings::Setting m_testSetting1 = AppSettings::Setting("Name2", "Simon");
	AppSettings::Setting m_testSetting2 = AppSettings::Setting("Name3", "Kurt");
	AppSettings::Setting m_testSetting3 = AppSettings::Setting("Name4", "Hans");
	AppSettings::Setting m_testSetting4 = AppSettings::Setting("Text", "Das ist ein Text mit mehreren\nZeilen");
	Group0 m_group0;
	Group1 m_group1;
};

class ThisAppSettings : public AppSettings::ApplicationSettings
{
public:
	ThisAppSettings()
		: ApplicationSettings("simpleSettings")
	{
		setPath("settings");

		addGroup(m_group2);
	}

	Group2 m_group2;
};

int main(int argc, char* argv[])
{
#ifdef QT_WIDGETS_ENABLED
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#ifdef QT_ENABLED
	QApplication app(argc, argv);
#endif
	//Log::UI::QConsoleView* console = new Log::UI::QConsoleView();
	//console->show();
	Log::UI::NativeConsoleView* console = new Log::UI::NativeConsoleView();


	AppSettings::LibraryInfo::printInfo();
	ThisAppSettings settings;
	settings.load();
	settings.save();
	AppSettings::UI::UI_AppSettingsEditor* editor = new AppSettings::UI::UI_AppSettingsEditor();
	editor->show();

	

#ifdef QT_ENABLED
	return app.exec();
#else
	return 0;
#endif
}
