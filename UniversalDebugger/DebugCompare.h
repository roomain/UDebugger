#pragma once

#include <QMainWindow>
#include "ui_DebugCompare.h"

class DebugCompare : public QMainWindow
{
	Q_OBJECT

public:
	DebugCompare(QWidget *parent = nullptr);
	~DebugCompare();

private:
	Ui::DebugCompareClass ui;
};
