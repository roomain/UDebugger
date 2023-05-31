#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <QVariant>
#include "qtmodels_global.h"

class ITreeNode;
using FilterTreeNode = std::function<bool(const std::shared_ptr<ITreeNode>&)>;
using SortTreeNode = std::function<bool(const std::weak_ptr<ITreeNode>&, const std::weak_ptr<ITreeNode>&)>;

/*@brief interface for TreeNode*/
class QTMODELS_EXPORT ITreeNode : public std::enable_shared_from_this<ITreeNode>
{
protected:
	std::weak_ptr<ITreeNode> m_pParentNode;
	std::vector<std::shared_ptr<ITreeNode>> m_vChildren;
	std::vector<std::weak_ptr<ITreeNode>> m_vDisplayedChildren;

public:
	ITreeNode() = default;
	ITreeNode(const std::shared_ptr<ITreeNode>& a_parent);
	virtual ~ITreeNode();
	void addChild(const std::shared_ptr<ITreeNode>& a_child);
	void addChildren(const std::vector<std::shared_ptr<ITreeNode>>& a_lchildren);
	void removeChild(const std::shared_ptr<ITreeNode>& a_child);
	void removeChild(const int a_index);
	[[nodiscard]] std::shared_ptr<ITreeNode> findItem(FilterTreeNode a_filter)const;
	void clear();
	void filterChildren(FilterTreeNode a_filter, const bool a_recursively);
	void sortChildren(SortTreeNode a_sorter, const bool a_recursively);
	[[nodiscard]] inline std::weak_ptr<ITreeNode> displayChildAt(const int a_index)const { return m_vDisplayedChildren.at(a_index); }
	[[nodiscard]] inline std::weak_ptr<ITreeNode> parent()const { return m_pParentNode; }
	[[nodiscard]] inline std::shared_ptr<ITreeNode> childAt(const int a_index)const { return m_vChildren.at(a_index); }
	[[nodiscard]] int indexDisplayedInParent()const;

	[[nodiscard]] virtual QVariant displayRoleData()const { return QVariant(); }
	[[nodiscard]] virtual QVariant decorationRoleData()const { return QVariant(); }
	[[nodiscard]] virtual QVariant foregroundRoleData()const { return QVariant(); }
	[[nodiscard]] virtual QVariant backgroundRoleData()const { return QVariant(); }
	[[nodiscard]] virtual QVariant fontRoleData()const { return QVariant(); }
	[[nodiscard]] virtual QVariant textAlignmentRoleData()const { return QVariant(); }
	[[nodiscard]] virtual QVariant checkStateRoleData()const { return QVariant(); }
	[[nodiscard]] virtual QVariant tooltipRoleData()const { return QVariant(); }
	[[nodiscard]] virtual QVariant userRoleData([[maybe_unused]]const int a_offset)const { return QVariant(); }

	using const_iterChild = std::vector<std::shared_ptr<ITreeNode>>::const_iterator;
	inline [[nodiscard]] const_iterChild cBeginChildren()const { return m_vChildren.cbegin(); }
	inline [[nodiscard]] const_iterChild cEndChildren()const { return m_vChildren.cend(); }
	inline [[nodiscard]] size_t countChildren()const noexcept { return m_vChildren.size(); }

	using const_iterDisplayChild = std::vector<std::weak_ptr<ITreeNode>>::const_iterator;
	inline [[nodiscard]] const_iterDisplayChild cBeginDisplayChildren()const { return m_vDisplayedChildren.cbegin(); }
	inline [[nodiscard]] const_iterDisplayChild cEndDisplayChildren()const { return m_vDisplayedChildren.cend(); }
	inline [[nodiscard]] size_t count()const noexcept { return m_vDisplayedChildren.size(); }
};