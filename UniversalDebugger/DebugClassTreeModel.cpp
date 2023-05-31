#include "DebugClassTreeModel.h"
#include "DebugObjectModelNode.h"
#include "DebugClientConnection.h"
#include <map>
#include <string>

void DebugClassTreeModel::setup(const InstanceList& a_instanceList)
{
	beginResetModel();
	m_pRoot->clear();
	std::map<std::string, std::shared_ptr<DebugFolderodelNode>> mpFolder;
	for (const auto& instance : a_instanceList)
	{
		auto iter = mpFolder.find(instance.m_classInfo.m_className);
		std::shared_ptr<DebugFolderodelNode> pFolder;
		if (iter == mpFolder.end())
		{
			pFolder = std::make_shared<DebugFolderodelNode>(QString::fromStdString(instance.m_classInfo.m_className));
			m_pRoot->addChild(pFolder);
			mpFolder[instance.m_classInfo.m_className] = pFolder;
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