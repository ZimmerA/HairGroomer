#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QApplication>
#include "opengl/glwidget.h"
#include "mainwindow.h"
#include "mvpmodel.h"
#include "mvppresenter.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle("plastique");
	QCoreApplication::setApplicationName("Hair Groomer");
	QCoreApplication::setOrganizationName("Adrian Zimmer");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);

	// Setup Default OpenGl Context Information
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	const QCommandLineOption multiple_sample_option("multisample", "Multisampling");
	parser.addOption(multiple_sample_option);
	const QCommandLineOption core_profile_option("coreprofile", "Use core profile");
	parser.addOption(core_profile_option);
	parser.process(a);
	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(24);

	if (parser.isSet(multiple_sample_option))
		fmt.setSamples(4);
	if (parser.isSet(core_profile_option))
	{
		fmt.setVersion(3, 3);
		fmt.setProfile(QSurfaceFormat::CoreProfile);
	}

	// set swap interval to 0 to increase window resize performance
	fmt.setSwapInterval(0);

	QSurfaceFormat::setDefaultFormat(fmt);

	// Setup MVP Pattern
	MvpModel model;
	MvpPresenter presenter;
	presenter.set_model(&model);

	MainWindow view;
	view.set_presenter(&presenter);
	presenter.set_view(&view);

	view.show();

	// Load the default values of the ui control elements
	presenter.load_default_values();

	const auto result = QApplication::exec();
	return result;
}
