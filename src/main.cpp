#include "QtWidgets/QApplication"
#include "humblelogging/api.h"
#include "MsvcAppender.h"
#include "MainWindow.h"

void initLogging()
{
	using namespace humble::logging;

	auto& fac = Factory::getInstance();
	fac.setDefaultFormatter(new PatternFormatter("[%date][%lls][%filename] %m\n"));
	//fac.registerAppender(new RollingFileAppender(Poco::Path::temp() + "appmissionserver.log", true));
#ifdef WIN32 && _DEBUG
	fac.registerAppender(new MsvcAppender());
#else
	fac.registerAppender(new ConsoleAppender());
#endif
	fac.setConfiguration(DefaultConfiguration::createFromString(
		"logger.level(*)=debug\n"
		""));
}

int main(int argc, char *argv[])
{
	initLogging();

	QApplication qapp(argc, argv);
	qapp.setApplicationName("ProtoInfo");
	qapp.setOrganizationName("insaneFactory");
	qapp.setOrganizationDomain("http://insanefactory.com/");
	qapp.setQuitOnLastWindowClosed(true);

	MainWindow win;
	win.show();

	return qapp.exec();
}
