#pragma once
#include "ExplorerTreeView.h"
#include <qevent.h>

ExplorerTreeView::ExplorerTreeView(QWidget* a_parent) : QTreeView(a_parent)
{
	//
}

void ExplorerTreeView::mouseReleaseEvent(QMouseEvent* a_event)
{
	auto index = indexAt(a_event->pos());
	if (index.isValid() && a_event->button() == Qt::RightButton)
		emit sg_rightClicked(index.data(Qt::UserRole).toLongLong(), a_event->globalPos(), index);
	QTreeView::mouseReleaseEvent(a_event);
}
