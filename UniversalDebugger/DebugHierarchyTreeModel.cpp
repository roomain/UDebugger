#include "DebugHierarchyTreeModel.h"
#include "DebugObjectModelNode.h"
#include "DebugClientConnection.h"
#include <map>

void DebugHierarchyTreeModel::setup(const InstanceList& a_instanceList)
{
	beginResetModel();
	m_pRoot->clear();
	std::map<int64_t, std::shared_ptr<DebugObjectModelNode>> mpInstance;
	for (const auto& instance : a_instanceList)
	{
		auto pItem = std::make_shared<DebugObjectModelNode>(instance);
		if (instance.m_ownerUID <= 0)
		{
			m_pRoot->addChild(pItem);
		}
		else
		{
			auto iter = mpInstance.find(instance.m_ownerUID);
			if (iter == mpInstance.end())
			{
				m_pRoot->addChild(pItem);
			}
			else
			{
				iter->second->addChild(pItem);
			}
		}
		mpInstance[instance.m_uid] = pItem;
	}
	m_pRoot->filterChildren(nullptr, true);
	m_pRoot->sortChildren(nullptr, true);
	endResetModel();
}

void DebugHierarchyTreeModel::ownerChange(const int64_t& a_uid, const int64_t& a_owner)
{
	beginResetModel();
	auto pItem = m_pRoot->findItem([&](const std::shared_ptr<ITreeNode>& a_item)
		{
			return std::dynamic_pointer_cast<DebugObjectModelNode>(a_item)->uid() == a_uid;
		});

	auto pOwnerItem = m_pRoot->findItem([&](const std::shared_ptr<ITreeNode>& a_item)
		{
			return std::dynamic_pointer_cast<DebugObjectModelNode>(a_item)->uid() == a_owner;
		});

	if (pOwnerItem)
	{
		pOwnerItem->addChild(pItem);
	}
	else
	{
		m_pRoot->addChild(pItem);
	}

	endResetModel();
}