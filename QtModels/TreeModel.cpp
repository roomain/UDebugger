#include "TreeModel.h"
#include <qdebug.h>

TreeModel::TreeModel() : QAbstractItemModel()
{
	m_pRoot = std::make_shared<ITreeNode>();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
	const ITreeNode* pItem = static_cast<const ITreeNode*>(parent.internalPointer());
	return createIndex(row, column,
		pItem ? pItem->displayChildAt(row).lock().get() : m_pRoot->displayChildAt(row).lock().get());

}

QModelIndex TreeModel::parent(const QModelIndex& child) const
{
	const ITreeNode* pItem = static_cast<const ITreeNode*>(child.internalPointer());
	if (pItem == nullptr || pItem == m_pRoot.get())
		return QModelIndex();

	if(pItem->parent().lock() == m_pRoot)
		return QModelIndex();

	return createIndex(pItem->parent().lock()->indexDisplayedInParent(), 0, pItem->parent().lock().get());
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
	const ITreeNode* pItem = static_cast<const ITreeNode*>(parent.internalPointer());
	return static_cast<int>(pItem ? pItem->count() : m_pRoot->count());
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
	return 1;
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
	QVariant var;
	if (auto pItem = static_cast<const ITreeNode*>(index.internalPointer()))
	{
		switch (role)
		{
		case Qt::DisplayRole:
			var = pItem->displayRoleData();
			break;

		case Qt::DecorationRole:
			var = pItem->decorationRoleData();
			break;

		case Qt::ForegroundRole:
			var = pItem->foregroundRoleData();
			break;

		case Qt::ToolTipRole:
			var = pItem->tooltipRoleData();
			break;

		case Qt::TextAlignmentRole:
			var = pItem->textAlignmentRoleData();
			break;

		case Qt::FontRole:
			var = pItem->fontRoleData();
			break;

		case Qt::CheckStateRole:
			var = pItem->checkStateRoleData();
			break;

		default:
			if (role >= Qt::UserRole)
				var = pItem->userRoleData(role - static_cast<int>(Qt::UserRole));
			break;
		}
	}
	return var;
}


void TreeModel::filter(QModelIndex a_from, FilterTreeNode a_filter, const bool a_recursively)
{
	if (a_from.model() == this || !a_from.isValid())
	{
		beginResetModel();
		if (auto pItem = static_cast<ITreeNode*>(a_from.internalPointer()))
		{
			pItem->filterChildren(a_filter, a_recursively);
		}
		else
		{
			m_pRoot->filterChildren(a_filter, a_recursively);
		}
		endResetModel();
	}
}

void TreeModel::sort(QModelIndex a_from, SortTreeNode a_sorter, const bool a_recursively)
{
	if (a_from.model() == this)
	{
		beginResetModel();
		if (auto pItem = static_cast<ITreeNode*>(a_from.internalPointer()))
		{
			pItem->sortChildren(a_sorter, a_recursively);
		}
		else
		{
			m_pRoot->sortChildren(a_sorter, a_recursively);
		}
		endResetModel();
	}
}