#pragma once

#include <QWidget>
#include "ui_DebugFilter.h"

class DebugFilter : public QWidget
{
	Q_OBJECT

public:
	DebugFilter(QWidget *parent = nullptr);
	~DebugFilter();

private:
	Ui::DebugFilterClass ui;
	void onSearch();
};
