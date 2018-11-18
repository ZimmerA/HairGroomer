
#include <QApplication>
#include <QMessageBox>
#include <QOpenGLFunctions_3_3_Core>
#include <QCommandLineParser>

#include "mainwindow.h"
#include "mvpmodel.h"
#include "mvppresenter.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle("plastique");
	QCoreApplication::setApplicationName("Hair Groomer");
	QCoreApplication::setOrganizationName("Adrian Zimmer");
	QCoreApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption();
	const QCommandLineOption fbx_model_option("fbx", QCoreApplication::translate("main", "Load the provided model on application startup"), QCoreApplication::translate("main", "path"));
	parser.addOption(fbx_model_option);

	// Process the actual command line arguments given by the user
	parser.process(a);

	// OpenGL version
	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(24);
	fmt.setVersion(3, 3);
	fmt.setProfile(QSurfaceFormat::CoreProfile);

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

	QOpenGLContext c;

	// Check if OpenGL 3.3 core context could be created, if not exit with Error.
	auto *funcs = c.versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (!funcs)
	{
		QMessageBox::critical(nullptr, "Error", "Failed creating OpenGL 3.3 context. This can be caused by an outdated GPU or GPU drivers.");
		exit(1);
	}

	view.show();
	
	const UiSettings defaultsettings;
	// Load the default values of the ui control elements
	view.set_ui_settings(defaultsettings);
	
	if (!parser.value(fbx_model_option).isEmpty())
		presenter.load_fbx_model(parser.value(fbx_model_option));

	const auto result = QApplication::exec();

	return result;
}
