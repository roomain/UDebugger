#include "ITreeNode.h"
#include <ranges>

ITreeNode::ITreeNode(const std::shared_ptr<ITreeNode>& a_parent) : m_pParentNode{ a_parent }
{
	a_parent->m_vChildren.push_back(shared_from_this());
}

ITreeNode::~ITreeNode()
{
	//
}

void ITreeNode::addChild(const std::shared_ptr<ITreeNode>& a_child)
{
	if (a_child->m_pParentNode.lock())
		a_child->m_pParentNode.lock()->removeChild(a_child);

	m_vChildren.push_back(a_child);
	a_child->m_pParentNode = shared_from_this();
}

void ITreeNode::addChildren(const std::vector<std::shared_ptr<ITreeNode>>& a_lchildren)
{
	for (auto& pChildren : a_lchildren)
		addChild(pChildren);
}

void ITreeNode::removeChild(const std::shared_ptr<ITreeNode>& a_child)
{
	auto iterChild = std::ranges::find(m_vChildren, a_child);
	if (iterChild != m_vChildren.end())
		m_vChildren.erase(iterChild);

	auto iterDisplayedChild = std::ranges::find_if(m_vDisplayedChildren, [&a_child](auto&& a_pDisplayChild)
		{
			return a_pDisplayChild.lock() == a_child;
		});
	if (iterDisplayedChild != m_vDisplayedChildren.end())
		m_vDisplayedChildren.erase(iterDisplayedChild);

	a_child->m_pParentNode.reset();
}

void ITreeNode::removeChild(const int a_index)
{
	if (m_vDisplayedChildren.size() > a_index)
	{
		auto iter = m_vDisplayedChildren.begin() + a_index;
		
		auto pChild = (*iter).lock();
		if (pChild)
			pChild->m_pParentNode.reset();

		auto iterChild = std::ranges::find(m_vChildren, (*iter).lock());
		if (iterChild != m_vChildren.end())
			m_vChildren.erase(iterChild);

		m_vDisplayedChildren.erase(iter);
	}
}

void ITreeNode::clear()
{
	for (auto& pChild : m_vChildren)
		pChild->m_pParentNode.reset();
	m_vChildren.clear();
	m_vDisplayedChildren.clear();
}

std::shared_ptr<ITreeNode> ITreeNode::findItem(FilterTreeNode a_filter)const
{
	std::shared_ptr<ITreeNode> pItem;
	for (auto& pChild : m_vChildren)
	{
		if (a_filter(pChild))
		{
			pItem = pChild;
		}
		else
		{
			pItem = pChild->findItem(a_filter);
		}

		if (pItem)
			break;
	}
	return pItem;
}

void ITreeNode::filterChildren(FilterTreeNode a_filter, const bool a_recursively)
{
	m_vDisplayedChildren.clear();
	if (a_filter)
	{
		for (auto& pChild : m_vChildren | std::views::filter(a_filter))
			m_vDisplayedChildren.emplace_back(pChild);
	}
	else
	{
		for (auto& pChild : m_vChildren)
			m_vDisplayedChildren.emplace_back(pChild);
	}

	if(a_recursively)
		for (auto& pChild : m_vDisplayedChildren)
			pChild.lock()->filterChildren(a_filter, true);
}

void ITreeNode::sortChildren(SortTreeNode a_sorter, const bool a_recursively)
{
	if (a_sorter)
		std::sort(m_vDisplayedChildren.begin(), m_vDisplayedChildren.end(), a_sorter);
	if (a_recursively)
	{
		for (auto& pChild : m_vDisplayedChildren)
			pChild.lock()->sortChildren(a_sorter, a_recursively);
	}
}

int ITreeNode::indexDisplayedInParent()const
{
	int index = -1;
	if (m_pParentNode.lock())
	{
		auto pSharedThis = shared_from_this();
		auto iterChild = std::ranges::find_if(m_pParentNode.lock()->m_vDisplayedChildren, 
			[&pSharedThis](auto&& pDisplayed)
			{
				return pDisplayed.lock() == pSharedThis;
			});
		index = iterChild - m_pParentNode.lock()->m_vDisplayedChildren.begin();
	}
	return index;
}