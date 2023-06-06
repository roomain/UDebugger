#pragma once
#include "DebugProtocol.h"
#include <list>
#include <vector>
#include <qmetatype.h>

using LogsList = std::list<DebugProtocol::LogMessage>;
using InstanceList = std::list<DebugProtocol::Instance>;
using VarList = std::vector<DebugProtocol::VariableData>;
using ComparisonData = DebugProtocol::ClassCompareData;

Q_DECLARE_METATYPE(InstanceList)
Q_DECLARE_METATYPE(VarList)
Q_DECLARE_METATYPE(ComparisonData)