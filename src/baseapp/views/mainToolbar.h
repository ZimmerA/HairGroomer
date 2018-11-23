#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

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

#endif