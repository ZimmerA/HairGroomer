#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

class MvpPresenter;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	// Getters/Setters
	void setPresenter(MvpPresenter * presenter) { this->presenter = presenter;}
	MvpPresenter * getPresenter() const { return presenter; }

private:
	Ui::MainWindow *ui;
	MvpPresenter * presenter;

};

#endif // MAINWINDOW_H
