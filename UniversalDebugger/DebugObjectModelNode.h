#pragma once
#include "ITreeNode.h"
#include "DebugProtocol.h"

class DebugFolderodelNode : public ITreeNode
{
private:
	QString m_folderName;

public:
	DebugFolderodelNode(const QString& a_name);
	~DebugFolderodelNode() override = default;
	[[nodiscard]] QVariant displayRoleData()const final;
	[[nodiscard]] QVariant decorationRoleData()const final;
	[[nodiscard]] QVariant tooltipRoleData()const final;
	[[nodiscard]] QVariant fontRoleData()const final;
	[[nodiscard]] QVariant userRoleData([[maybe_unused]] const int a_offset)const final{ return -1; }
};

class DebugObjectModelNode : public ITreeNode
{
private:
	DebugProtocol::Instance m_objInstance;

public:
	DebugObjectModelNode() = delete;
	~DebugObjectModelNode() override = default;
	DebugObjectModelNode(const DebugProtocol::Instance& a_data);
	inline int64_t uid()const { return m_objInstance.m_uid; }
	inline const DebugProtocol::Instance& data()const noexcept { return m_objInstance; }

	[[nodiscard]] QVariant displayRoleData()const final;
	[[nodiscard]] QVariant decorationRoleData()const final;
	[[nodiscard]] QVariant foregroundRoleData()const final;
	[[nodiscard]] QVariant backgroundRoleData()const final;
	[[nodiscard]] QVariant fontRoleData()const final;
	[[nodiscard]] QVariant textAlignmentRoleData()const final;
	[[nodiscard]] QVariant checkStateRoleData()const final;
	[[nodiscard]] QVariant tooltipRoleData()const final;
	[[nodiscard]] QVariant userRoleData([[maybe_unused]] const int a_offset)const final;
};

