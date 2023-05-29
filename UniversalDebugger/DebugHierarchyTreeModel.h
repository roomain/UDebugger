#pragma once
#include "TreeModel.h"
#include "defines.h"

class DebugHierarchyTreeModel : public TreeModel
{
	Q_OBJECT
public:
	DebugHierarchyTreeModel() = default;
	~DebugHierarchyTreeModel() override = default;
	void setup(const InstanceList& a_instanceList);
};

