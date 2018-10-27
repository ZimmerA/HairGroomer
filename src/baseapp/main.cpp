#include <QApplication>
#include "mainwindow.h"
#include "mvpmodel.h"
#include "mvppresenter.h"
#include <QMessageBox>
#include <QOpenGLFunctions_3_3_Core>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QApplication::setStyle("plastique");
	QCoreApplication::setApplicationName("Hair Groomer");
	QCoreApplication::setOrganizationName("Adrian Zimmer");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);

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
	auto* funcs = c.versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (! funcs)
	{
		QMessageBox::critical(0, "Error", "Error creating opengl context for version 3.3. The reason for this might be outdated graphics drivers or that your gpu is too old. Closing application.");
		exit(1);
	}

	view.show();
	UiSettings defaultsettings;
	// Load the default values of the ui control elements
	presenter.load_ui_values(defaultsettings);
	const auto result = QApplication::exec();
	return result;
}
