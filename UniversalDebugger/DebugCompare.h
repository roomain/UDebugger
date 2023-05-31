#pragma once

#include <QMainWindow>
#include "ui_DebugCompare.h"

class DebugCompare : public QMainWindow
{
	Q_OBJECT

public:
	DebugCompare(QWidget *parent = nullptr);
	~DebugCompare();
	void compare(const int64_t& a_left, const int64_t& a_right);

private:
	Ui::DebugCompareClass ui;
	void onAll();
	void onlyEqual();
	void onlyNotEqual();
};
