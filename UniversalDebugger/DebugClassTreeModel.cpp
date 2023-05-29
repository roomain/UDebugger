#include "DebugClassTreeModel.h"
#include "DebugObjectModelNode.h"
#include "DebugClientConnection.h"
#include <map>

void DebugClassTreeModel::setup(const InstanceList& a_instanceList)
{
	beginResetModel();
	m_pRoot->clear();
	std::map<QString, std::shared_ptr<DebugFolderodelNode>> mpFolder;
	for (const auto& instance : a_instanceList)
	{
		auto iter = mpFolder.find(instance.m_sClassName);
		std::shared_ptr<DebugFolderodelNode> pFolder;
		if (iter == mpFolder.end())
		{
			pFolder = std::make_shared<DebugFolderodelNode>(instance.m_sClassName);
			m_pRoot->addChild(pFolder);
		}
		else
		{
			pFolder = iter->second;
		}

		auto pItem = std::make_shared<DebugObjectModelNode>(instance);
		pFolder->addChild(pItem);
	}
	m_pRoot->filterChildren(nullptr, true);
	m_pRoot->sortChildren(nullptr, true);
	endResetModel();
}