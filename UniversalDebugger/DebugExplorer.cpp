#include "DebugExplorer.h"
#include "DebugHierarchyTreeModel.h"
#include "DebugClassTreeModel.h"
#include "DebugClientConnection.h"
#include "DebugProperties.h"
#include "DebugObjectModelNode.h"
#include <qscrollbar.h>
#include <QRegularExpression>


int DebugExplorer::m_iIndex = 0;

DebugExplorer::DebugExplorer(QWidget* parent)
	: QMainWindow(parent)
{
	m_iUID = m_iIndex++;
	ui.setupUi(this);
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

}


DebugExplorer::~DebugExplorer()
{}

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
