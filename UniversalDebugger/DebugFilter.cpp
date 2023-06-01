#include "DebugFilter.h"
#include <QRegularExpressionValidator>

DebugFilter::DebugFilter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.cboSearch->setValidator(new QRegularExpressionValidator);
	QObject::connect(ui.cboSearch, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DebugFilter::onSearch);
	QObject::connect(ui.cboSearch, QOverload<const QString&>::of(&QComboBox::textActivated), this, &DebugFilter::onSearch);
	QObject::connect(ui.pBtnSearch, QOverload<bool>::of(&QPushButton::clicked), this, &DebugFilter::onSearch);
	QObject::connect(ui.pBtnReset, QOverload<bool>::of(&QPushButton::clicked), this, &DebugFilter::onReset);
}

DebugFilter::~DebugFilter()
{}

void DebugFilter::onReset()
{
	ui.cboSearch->setCurrentText("");
	onSearch();
}

void DebugFilter::onSearch()
{
	auto filterStr = ui.cboSearch->currentText();

	if (ui.cboSearch->findText(filterStr) < 0)
		ui.cboSearch->insertItem(0, filterStr);

	emit sg_search(filterStr);
}