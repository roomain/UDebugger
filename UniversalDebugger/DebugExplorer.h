#pragma once

#include <QMainWindow>
#include <QMenu>
#include "ui_DebugExplorer.h"
#include "defines.h"
#include "classInfo.h"

class DebugClientConnection;
class ITreeNode;

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
	void sg_askCompare(const int, const int64_t&, const int64_t&);
	void sg_path(const QString&);

private:
	static int m_iIndex;
	int m_iUID;
	Ui::DebugExplorerClass ui;
	QMenu m_selectMenu;
	QString m_lastSelectedObject;
	int64_t m_compareLeft = -1;
	int64_t m_compareRight = -1;
	int64_t m_LastSelected = -1;

	void onCompareRefresh();
	void onRightClicked(const int64_t& a_UID, const QPoint& a_pos, const QModelIndex& a_index);
	void onCloseCompare();
	void onCompare(const int a_index, const ComparisonData& a_data0, const ComparisonData& a_data1);
	void onVariable(const int a_index, const int64_t& a_object, const Debugger::ClassInfo& info, const VarList& a_vars);
	void onItemClicked(const QModelIndex& a_index);
	void onSetupTree(const InstanceList& a_instanceList);
	void onTabChanged(int a_index);
	void onFilter(const QString& a_filter);

	void onRefreshTree();
	void onRefreshProps();

};
