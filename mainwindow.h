#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

#include <mvppresenter.h>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	// Getters/Setters
	void set_presenter(MvpPresenter* presenter) { this->m_presenter_ = presenter; }
	MvpPresenter* get_presenter() const { return m_presenter_; }

private slots:
	void on_actionExport_triggered();

private:
	Ui::MainWindow* m_ui_;
	MvpPresenter* m_presenter_{};
};

#endif // MAINWINDOW_H
