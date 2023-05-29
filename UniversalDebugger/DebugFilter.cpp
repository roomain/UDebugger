#include "DebugFilter.h"
#include <QRegularExpressionValidator>

DebugFilter::DebugFilter(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.cboSearch->setValidator(new QRegularExpressionValidator);
}

DebugFilter::~DebugFilter()
{}

void DebugFilter::onSearch()
{
	//
}