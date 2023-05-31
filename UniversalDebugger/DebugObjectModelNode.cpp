#include "DebugObjectModelNode.h"
#include "DebugWorkspace.h"
#include <QIcon>
#include <qfont.h>

DebugFolderodelNode::DebugFolderodelNode(const QString& a_name) : m_folderName{ a_name }
{
	//
}

QVariant DebugFolderodelNode::displayRoleData()const
{
	return m_folderName;
}

QVariant DebugFolderodelNode::decorationRoleData()const
{
	return QIcon(":/DebuggerUI/resources/folder.png");
}

QVariant DebugFolderodelNode::tooltipRoleData()const
{
	return QString("Instance count: %1").arg(countChildren());
}

QVariant DebugFolderodelNode::fontRoleData()const
{
	QFont font;
	font.setBold(true);
	return font;
}
//-------------------------------------------------------------------

DebugObjectModelNode::DebugObjectModelNode(const DebugProtocol::Instance& a_data) : m_objInstance{ a_data }
{
	//
}


QVariant DebugObjectModelNode::displayRoleData()const
{
	return m_objInstance.m_sName;
}

QVariant DebugObjectModelNode::decorationRoleData()const
{
	return QIcon(":/DebuggerUI/resources/object.png");
}

QVariant DebugObjectModelNode::foregroundRoleData()const
{
	return DebugWorkspace::instance().getColor(QString::fromStdString(m_objInstance.m_classInfo.m_className));
}

QVariant DebugObjectModelNode::backgroundRoleData()const
{
	return QVariant();
}

QVariant DebugObjectModelNode::fontRoleData()const
{
	return QVariant();
}

QVariant DebugObjectModelNode::textAlignmentRoleData()const
{
	return QVariant();
}

QVariant DebugObjectModelNode::checkStateRoleData()const
{
	return QVariant();
}

QVariant DebugObjectModelNode::tooltipRoleData()const
{
	return QString("UUID: %2").arg(m_objInstance.m_uid);
}

QVariant DebugObjectModelNode::userRoleData([[maybe_unused]] const int a_offset)const
{
	return m_objInstance.m_uid;
}
