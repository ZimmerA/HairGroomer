#pragma once

#include <QWidget>
#include <ui_mainToolbar.h>

class MainToolbar : public QWidget
{
	Q_OBJECT

public:
	MainToolbar(QWidget *parent = Q_NULLPTR);
	~MainToolbar();

private:
	Ui::MainToolbar m_ui_;
};
