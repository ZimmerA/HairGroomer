#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_ui_(new Ui::MainWindow)
{
	m_ui_->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete m_ui_;
}

void MainWindow::on_actionExport_triggered()
{
	QString file_name = QFileDialog::getSaveFileName(this,
	                                                tr("Export Hair"), "",
	                                                tr("Nvidia APX File (*.apx);;All Files (*)"));

	if (file_name.isEmpty())
		return;
	QFile file(file_name);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::information(this, tr("Unable to open file"),
		                         file.errorString());
		return;
	}

	// File could be opened delegate exporting to presenter
	m_presenter_->export_hair(&file);
}
