#include "DebugCompare.h"

DebugCompare::DebugCompare(QWidget *parent)
	: QMainWindow(parent), m_actGrp{this}
{
	ui.setupUi(this);

	auto pPropsModel = new DebugProperties;
	ui.twLeft->setModel(pPropsModel);
	ui.twLeft->setItemDelegate(new PropertiesDelegate);

	pPropsModel = new DebugProperties;
	ui.twRight->setModel(pPropsModel);
	ui.twRight->setItemDelegate(new PropertiesDelegate);

	m_actGrp.addAction(ui.actionAll);
	m_actGrp.addAction(ui.actionOnly_same);
	m_actGrp.addAction(ui.actionOnly_different);
	QObject::connect(ui.actionCLOSE, QOverload<bool>::of(&QAction::triggered), this, &DebugCompare::onClose);
	QObject::connect(ui.actionAll, QOverload<bool>::of(&QAction::triggered), this, &DebugCompare::onAll);
	QObject::connect(ui.actionOnly_same, QOverload<bool>::of(&QAction::triggered), this, &DebugCompare::onlyEqual);
	QObject::connect(ui.actionOnly_different, QOverload<bool>::of(&QAction::triggered), this, &DebugCompare::onlyNotEqual);
	QObject::connect(ui.actionrefresh, QOverload<bool>::of(&QAction::triggered), this, &DebugCompare::refresh);
}

DebugCompare::~DebugCompare()
{}

void DebugCompare::setComparisonNames(const QString& a_left, const QString& a_right)
{
	ui.lblObjLeft->setText(a_left);
	ui.lblObjRight->setText(a_right);
}

void DebugCompare::compare(const ComparisonData& a_left, const ComparisonData& a_right)
{
	auto pModelLeft = dynamic_cast<DebugProperties*>(ui.twLeft->model());
	auto pModelRight = dynamic_cast<DebugProperties*>(ui.twRight->model());
	pModelLeft->setup(a_left, a_left.m_uid, a_left.m_variables);
	pModelRight->setup(a_right, a_right.m_uid, a_right.m_variables);

	if (ui.actionOnly_different->isChecked())
	{
		DebugProperties::compare(DebugProperties::CompareMode::Different, pModelLeft, pModelRight);
	}
	else if (ui.actionOnly_same->isChecked())
	{
		DebugProperties::compare(DebugProperties::CompareMode::Equal, pModelLeft, pModelRight);
	}
	else
	{
		DebugProperties::compare(DebugProperties::CompareMode::All, pModelLeft, pModelRight);
	}
}

void DebugCompare::onClose()
{
	//
	emit sg_close();
}

void DebugCompare::onAll()
{
	auto pModelLeft = dynamic_cast<DebugProperties*>(ui.twLeft->model());
	auto pModelRight = dynamic_cast<DebugProperties*>(ui.twRight->model());
	DebugProperties::compare(DebugProperties::CompareMode::All, pModelLeft, pModelRight);
}

void DebugCompare::onlyEqual()
{
	auto pModelLeft = dynamic_cast<DebugProperties*>(ui.twLeft->model());
	auto pModelRight = dynamic_cast<DebugProperties*>(ui.twRight->model());
	DebugProperties::compare(DebugProperties::CompareMode::Equal, pModelLeft, pModelRight);
}

void DebugCompare::onlyNotEqual()
{
	auto pModelLeft = dynamic_cast<DebugProperties*>(ui.twLeft->model());
	auto pModelRight = dynamic_cast<DebugProperties*>(ui.twRight->model());
	DebugProperties::compare(DebugProperties::CompareMode::Different, pModelLeft, pModelRight);
}

void DebugCompare::refresh()
{
	emit sg_refresh();
}