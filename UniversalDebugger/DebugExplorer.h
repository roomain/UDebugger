#pragma once

#include <QMainWindow>
#include "ui_DebugExplorer.h"
#include "defines.h"
#include "classInfo.h"

class DebugClientConnection;

class DebugExplorer : public QMainWindow
{
	Q_OBJECT

public:
	DebugExplorer(QWidget *parent = nullptr);
	~DebugExplorer();
	inline [[nodiscard]] int uid()const { return m_iUID; }
	void connect(DebugClientConnection& a_connection);

signals:
	void sg_askInstances(const int);
	void sg_askProperties(const int, const int64_t&);
	void sg_path(const QString&);

private:
	static int m_iIndex;
	int m_iUID;
	Ui::DebugExplorerClass ui;

	void onVariable(const int a_index, const int64_t& a_object, const Debugger::ClassInfo& info, const VarList& a_vars);
	void onItemClicked(const QModelIndex& a_index);
	void onSetupTree(const InstanceList& a_instanceList);
	void onTabChanged(int a_index);

	void onRefreshTree();
	void onRefreshProps();
};
