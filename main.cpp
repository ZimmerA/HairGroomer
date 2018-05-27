#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSurfaceFormat>

#include <QApplication>

#include "glwidget.h"
#include "mainwindow.h"
#include "mvpmodel.h"
#include "mvppresenter.h"

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
    a.setStyle("plastique");
	QCoreApplication::setApplicationName("Hair Groomer");
	QCoreApplication::setOrganizationName("Adrian Zimmer");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	// Setup Default OpenGl Context Information
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	QCommandLineOption multipleSampleOption("multisample", "Multisampling");
	parser.addOption(multipleSampleOption);
	QCommandLineOption coreProfileOption("coreprofile", "Use core profile");
	parser.addOption(coreProfileOption);
	parser.process(a);
	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(24);

	if (parser.isSet(multipleSampleOption))
		fmt.setSamples(4);
	if (parser.isSet(coreProfileOption)) {
        fmt.setVersion(4, 0);
		fmt.setProfile(QSurfaceFormat::CoreProfile);

	}
    // Else the window resizing will be painfully slow
    fmt.setSwapInterval(0);

	QSurfaceFormat::setDefaultFormat(fmt);

	// Setup MVP Pattern
	MvpModel model;
	MvpPresenter presenter;
	presenter.setModel(&model);

	MainWindow view;
	view.setPresenter(&presenter);
	presenter.setView(&view);

	view.show();
	int result = a.exec();
	return result;
}


