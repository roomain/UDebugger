#pragma once
#include <qtreeview.h>

class ExplorerTreeView : public QTreeView
{
	Q_OBJECT
public:
	ExplorerTreeView(QWidget* a_parent = nullptr);
	~ExplorerTreeView() {}

protected:
	void mouseReleaseEvent(QMouseEvent* a_event) override;

signals:
	void sg_rightClicked(const int64_t&, const QPoint&, const QModelIndex&);
};
