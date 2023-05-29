#pragma once
#include "TreeModel.h"
#include "defines.h"

class DebugClassTreeModel : public TreeModel
{
	Q_OBJECT
public:
	DebugClassTreeModel() = default;
	~DebugClassTreeModel() override = default;
	void setup(const InstanceList& a_instanceList);
};

