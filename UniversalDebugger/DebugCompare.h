#pragma once

#include <QMainWindow>
#include "ui_DebugCompare.h"
#include "DebugProperties.h"
#include <qactiongroup.h>

class DebugCompare : public QMainWindow
{
	Q_OBJECT

public:
	explicit DebugCompare(QWidget *parent = nullptr);
	~DebugCompare();
	void compare(const ComparisonData& a_left, const ComparisonData& a_right);
	void setComparisonNames(const QString& a_left, const QString& a_right);

signals:
	void sg_close();
	void sg_refresh();

private:
	Ui::DebugCompareClass ui;
	QActionGroup m_actGrp;
	DebugProperties::CompareMode m_compare = DebugProperties::CompareMode::All;

	void onClose();
	void onAll();
	void onlyEqual();
	void onlyNotEqual();
	void refresh();
};
