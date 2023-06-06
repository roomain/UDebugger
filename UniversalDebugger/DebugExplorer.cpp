#include "DebugExplorer.h"
#include "DebugHierarchyTreeModel.h"
#include "DebugClassTreeModel.h"
#include "DebugClientConnection.h"
#include "DebugProperties.h"
#include "DebugObjectModelNode.h"
#include <qscrollbar.h>
#include <QRegularExpression>
#include <qevent.h>
#include <qmenu.h>
#include "DebugCompare.h"


int DebugExplorer::m_iIndex = 0;

DebugExplorer::DebugExplorer(QWidget* parent)
	: QMainWindow(parent), m_selectMenu(this)
{
	m_iUID = m_iIndex++;
	ui.setupUi(this);
	m_selectMenu.addAction(ui.actionLeft_for_comparison);
	m_selectMenu.addAction(ui.actionRight_for_comparison);
	m_selectMenu.addAction(ui.actionCompare);

	ui.tabWInstances->setTabPosition(QTabWidget::South);
	auto pClassModel = new DebugClassTreeModel;
	auto pHierarchyModel = new DebugHierarchyTreeModel;
	ui.tWClasses->setModel(pClassModel);
	ui.tWHierarchy->setModel(pHierarchyModel);
	auto pPropsModel = new DebugProperties;
	ui.tbProps->setModel(pPropsModel);
	ui.tbProps->setItemDelegate(new PropertiesDelegate);

	QObject::connect(ui.pBtnRefreshClass, QOverload<bool>::of(&QPushButton::clicked), this, &DebugExplorer::onRefreshTree);
	QObject::connect(ui.pBtnRefreshProp, QOverload<bool>::of(&QPushButton::clicked), this, &DebugExplorer::onRefreshProps);
	QObject::connect(ui.pBtnRefreshHierarchy, QOverload<bool>::of(&QPushButton::clicked), this, &DebugExplorer::onRefreshTree);

	QObject::connect(ui.tWClasses, &QTreeView::clicked, this, &DebugExplorer::onItemClicked);
	QObject::connect(ui.tWHierarchy, &QTreeView::clicked, this, &DebugExplorer::onItemClicked);
	QObject::connect(ui.tabWInstances, &QTabWidget::currentChanged, this, &DebugExplorer::onTabChanged);

	QObject::connect(ui.filterHierarchy, &DebugFilter::sg_search, this, &DebugExplorer::onFilter);
	QObject::connect(ui.filterClasses, &DebugFilter::sg_search, this, &DebugExplorer::onFilter);
	QObject::connect(ui.filterProp, &DebugFilter::sg_search, this, &DebugExplorer::onFilter);


	QObject::connect(ui.tWClasses, &ExplorerTreeView::sg_rightClicked, this, &DebugExplorer::onRightClicked);
	QObject::connect(ui.tWHierarchy, &ExplorerTreeView::sg_rightClicked, this, &DebugExplorer::onRightClicked);


	QObject::connect(ui.pageCompare, &DebugCompare::sg_close, this, &DebugExplorer::onCloseCompare);
}


DebugExplorer::~DebugExplorer()
{}

void DebugExplorer::onCloseCompare()
{
	ui.stackedWidget->setCurrentIndex(0);
}

void DebugExplorer::onCompareRefresh()
{
	emit sg_askCompare(m_iUID, m_compareRight, m_compareRight);
}

void DebugExplorer::onRightClicked(const int64_t& a_UID, const QPoint& a_pos, const QModelIndex& a_index)
{
	if (a_UID < 0)
		return;

	onItemClicked(a_index);
	m_selectMenu.move(a_pos);
	ui.actionCompare->setVisible(m_compareLeft > -1 || m_compareRight > -1);
	auto pAct = m_selectMenu.exec();
	if (pAct)
	{
		if (pAct == ui.actionLeft_for_comparison)
		{
			m_compareLeft = a_UID;
			m_LastSelected = m_compareLeft;
			m_lastSelectedObject = a_index.model()->data(a_index, Qt::DisplayRole).toString();
		}
		else if (pAct == ui.actionRight_for_comparison)
		{
			m_compareRight = a_UID;
			m_LastSelected = m_compareRight;
			m_lastSelectedObject = a_index.model()->data(a_index, Qt::DisplayRole).toString();
		}
		else if (m_compareRight == m_LastSelected)
		{
			m_compareLeft = a_UID;
			m_LastSelected = m_compareLeft;
			ui.pageCompare->setComparisonNames(a_index.model()->data(a_index, Qt::DisplayRole).toString(), m_lastSelectedObject);
			emit sg_askCompare(m_iUID, m_compareLeft, m_compareRight);
			ui.stackedWidget->setCurrentIndex(1);
			auto pClassModel = static_cast<DebugClassTreeModel*>(ui.tWClasses->model());
		}
		else
		{
			m_compareRight = a_UID;
			m_LastSelected = m_compareRight;
			ui.pageCompare->setComparisonNames(m_lastSelectedObject, a_index.model()->data(a_index, Qt::DisplayRole).toString());
			emit sg_askCompare(m_iUID, m_compareLeft, m_compareRight);
			ui.stackedWidget->setCurrentIndex(1);
		}
	}
}

void DebugExplorer::onFilter(const QString& a_filter)
{
	QRegularExpression regExp(a_filter);

	FilterTreeNode filterTree = [&regExp](const std::shared_ptr<ITreeNode>& a_pNode)
	{
		auto pNode = std::dynamic_pointer_cast<DebugObjectModelNode>(a_pNode);
		if (pNode)
			return regExp.match(pNode->data().m_sName).hasMatch();
		return true;
	};

	if (sender() == ui.filterHierarchy)
	{
		auto pHierarchyModel = static_cast<DebugHierarchyTreeModel*>(ui.tWHierarchy->model());
		pHierarchyModel->filter(QModelIndex(), a_filter.isEmpty() ? nullptr : filterTree, true);
	}
	else if (sender() == ui.filterClasses)
	{
		auto pClassModel = static_cast<DebugClassTreeModel*>(ui.tWClasses->model());
		pClassModel->filter(QModelIndex(), a_filter.isEmpty() ? nullptr : filterTree, true);
	}
	else
	{
		auto pPropsModel = static_cast<DebugProperties*>(ui.tbProps->model());
		pPropsModel->filter(regExp);
	}
}

void DebugExplorer::onTabChanged(int a_index)
{
	if (a_index == 0)
	{
		onItemClicked(ui.tWHierarchy->currentIndex());
	}
	else
	{
		onItemClicked(ui.tWClasses->currentIndex());
	}
}

void DebugExplorer::onItemClicked(const QModelIndex& a_index)
{
	auto pItem = dynamic_cast<DebugObjectModelNode*>(static_cast<ITreeNode*>(a_index.internalPointer()));
	if (pItem)
	{
		emit sg_askProperties(m_iIndex, pItem->data().m_uid);
		QString path = pItem->displayRoleData().toString();
		auto parent = pItem->parent();
		while (parent.lock())
		{
			auto name = parent.lock()->displayRoleData().toString();
			if (!name.isEmpty())
				path = name + ">" + path;

			parent = parent.lock()->parent();
		}
		emit sg_path(QString("[Explorer] %1").arg(path));
	}
	else
	{
		emit sg_path("[Explorer]");
	}
}

void DebugExplorer::onCompare(const int a_index, const ComparisonData& a_data0, const ComparisonData& a_data1)
{
	if (a_index == m_iUID)
		ui.pageCompare->compare(a_data0, a_data1);
}

void DebugExplorer::onVariable(const int a_index, const int64_t& a_object, const Debugger::ClassInfo& info, const VarList& a_vars)
{
	if (m_iIndex == a_index)
	{
		auto pPropsModel = static_cast<DebugProperties*>(ui.tbProps->model());
		pPropsModel->setup(info, a_object, a_vars);
	}
}

void DebugExplorer::onSetupTree(const InstanceList& a_instanceList)
{
	m_compareLeft = -1;
	m_compareRight = -1;
	m_LastSelected = -1;
	auto pClassModel = static_cast<DebugClassTreeModel*>(ui.tWClasses->model());
	auto pHierarchyModel = static_cast<DebugHierarchyTreeModel*>(ui.tWHierarchy->model());
	
	auto val = ui.tWClasses->verticalScrollBar()->value();
	// TODO
	pClassModel->setup(a_instanceList);
	// TODO
	ui.tWClasses->verticalScrollBar()->setValue(val);

	val = ui.tWHierarchy->verticalScrollBar()->value();
	// TODO
	pHierarchyModel->setup(a_instanceList);
	// TODO
	ui.tWHierarchy->verticalScrollBar()->setValue(val);
}

void DebugExplorer::connect(DebugClientConnection& a_connection)
{
	a_connection.onAskTree(m_iIndex);

	PropertiesDelegate* pPropDelegate = static_cast<PropertiesDelegate*>(ui.tbProps->itemDelegate());
	QObject::connect(pPropDelegate, &PropertiesDelegate::sg_writeProperty, &a_connection, &DebugClientConnection::onWriteProp);

	QObject::connect(this, &DebugExplorer::sg_askInstances, &a_connection, &DebugClientConnection::onAskTree);

	QObject::connect(&a_connection, &DebugClientConnection::sg_instances, this, &DebugExplorer::onSetupTree);
	QObject::connect(&a_connection, &DebugClientConnection::sg_variables, this, &DebugExplorer::onVariable);
	QObject::connect(this, &DebugExplorer::sg_askProperties, &a_connection, &DebugClientConnection::onAskProps);
	QObject::connect(this, &DebugExplorer::sg_askCompare, &a_connection, &DebugClientConnection::onAskCompare);
	QObject::connect(&a_connection, &DebugClientConnection::sg_compare, this, &DebugExplorer::onCompare);
}

void DebugExplorer::onRefreshTree()
{
	emit sg_askInstances(m_iIndex);
}

void DebugExplorer::onRefreshProps()
{
	auto pPropsModel = static_cast<DebugProperties*>(ui.tbProps->model());
	emit sg_askProperties(m_iIndex, pPropsModel->uid());
}
