#include "DebugProperties.h"
#include <QModelIndex>
#include <QColor>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qjsonarray.h>
#include <limits>

QWidget* PropertiesDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QWidget* pWidget = nullptr;
	auto pModel = static_cast<const DebugProperties*>(index.model());
	switch (pModel->m_vMembersVar.at(pModel->m_vDisplayedIndex.at(index.row())).m_type)
	{
	case Debugger::EVarType::type_bool:
	{
		auto pCbo = new QComboBox(parent);
		pCbo->addItem("false");
		pCbo->addItem("true");
		pWidget = pCbo;
		break;
	}

	case Debugger::EVarType::type_short:
	{
		auto pSpBx = new QSpinBox(parent);
		pSpBx->setRange(std::numeric_limits<short>::min(), std::numeric_limits<short>::max());
		pWidget = pSpBx;
		break;
	}

	case Debugger::EVarType::type_ushort:
	{
		auto pSpBx = new QSpinBox(parent);
		pSpBx->setRange(std::numeric_limits<ushort>::min(), std::numeric_limits<ushort>::max());
		pWidget = pSpBx;
		break;
	}

	case Debugger::EVarType::type_int:
	{
		auto pSpBx = new QSpinBox(parent);
		pSpBx->setRange(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
		pWidget = pSpBx;
		break;
	}

	case Debugger::EVarType::type_uint:
	{
		auto pSpBx = new QSpinBox(parent);
		pSpBx->setRange(std::numeric_limits<uint>::min(), std::numeric_limits<uint>::max());
		pWidget = pSpBx;
		break;
	}

	case Debugger::EVarType::type_float:
	{
		auto pSpBx = new QDoubleSpinBox(parent);
		pSpBx->setRange(std::numeric_limits<float>::min(), std::numeric_limits<float>::max());
		pSpBx->setDecimals(5);
		pSpBx->setSingleStep(0.01);
		pWidget = pSpBx;
		break;
	}

	case Debugger::EVarType::type_double:
	{
		auto pSpBx = new QDoubleSpinBox(parent);
		pSpBx->setRange(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
		pSpBx->setDecimals(5);
		pSpBx->setSingleStep(0.001);
		pWidget = pSpBx;
		break;
	}

	case Debugger::EVarType::type_string:
	{
		auto pLedt = new QLineEdit(parent);
		pWidget = pLedt;
		break;
	}

	default:
		break;
	}
	return pWidget;
}

void PropertiesDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	auto pModel = static_cast<const DebugProperties*>(index.model());
	QVariant varValue = pModel->m_vMembersVar.at(pModel->m_vDisplayedIndex.at(index.row())).m_value;
	switch (pModel->m_vMembersVar.at(pModel->m_vDisplayedIndex.at(index.row())).m_type)
	{
	case Debugger::EVarType::type_bool:
	{
		static_cast<QComboBox*>(editor)->setCurrentIndex(varValue.toBool() ? 0 : 1);
		break;
	}

	case Debugger::EVarType::type_short:
	{
		static_cast<QSpinBox*>(editor)->setValue(varValue.toInt());
		break;
	}

	case Debugger::EVarType::type_ushort:
	{
		static_cast<QSpinBox*>(editor)->setValue(varValue.toUInt());
		break;
	}

	case Debugger::EVarType::type_int:
	{
		static_cast<QSpinBox*>(editor)->setValue(varValue.toInt());
		break;
	}

	case Debugger::EVarType::type_uint:
	{
		static_cast<QSpinBox*>(editor)->setValue(varValue.toUInt());
		break;
	}

	case Debugger::EVarType::type_float:
	{
		static_cast<QDoubleSpinBox*>(editor)->setValue(varValue.toFloat());
		break;
	}

	case Debugger::EVarType::type_double:
	{
		static_cast<QDoubleSpinBox*>(editor)->setValue(varValue.toDouble());
		break;
	}

	case Debugger::EVarType::type_string:
	{
		static_cast<QLineEdit*>(editor)->setText(varValue.toString());
		break;
	}

	default:
		break;
	}
}

void PropertiesDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index)const
{
	DebugProperties* const pModel = static_cast<DebugProperties* const>(model);
	int propIndex = pModel->m_vDisplayedIndex.at(index.row());
	switch (pModel->m_vMembersVar.at(pModel->m_vDisplayedIndex.at(index.row())).m_type)
	{
	case Debugger::EVarType::type_bool:
	{
		pModel->m_vMembersVar[propIndex].m_value = static_cast<QComboBox*>(editor)->currentIndex() == 1;
		break;
	}

	case Debugger::EVarType::type_short:
	{
		pModel->m_vMembersVar[propIndex].m_value = static_cast<short>(static_cast<QSpinBox*>(editor)->value());
		break;
	}

	case Debugger::EVarType::type_ushort:
	{
		pModel->m_vMembersVar[propIndex].m_value = static_cast<ushort>(static_cast<QSpinBox*>(editor)->value());
		break;
	}

	case Debugger::EVarType::type_int:
	{
		pModel->m_vMembersVar[propIndex].m_value = static_cast<QSpinBox*>(editor)->value();
		break;
	}

	case Debugger::EVarType::type_uint:
	{
		pModel->m_vMembersVar[propIndex].m_value = static_cast<uint>(static_cast<QSpinBox*>(editor)->value());
		break;
	}

	case Debugger::EVarType::type_float:
	{
		pModel->m_vMembersVar[propIndex].m_value = static_cast<float>(static_cast<QDoubleSpinBox*>(editor)->value());
		break;
	}

	case Debugger::EVarType::type_double:
	{
		pModel->m_vMembersVar[propIndex].m_value = static_cast<QDoubleSpinBox*>(editor)->value();
		break;
	}

	case Debugger::EVarType::type_string:
	{
		pModel->m_vMembersVar[propIndex].m_value = static_cast<QLineEdit*>(editor)->text();
		break;
	}

	default:
		break;
	}
	emit sg_writeProperty(pModel->m_uid, propIndex, pModel->m_vMembersVar[propIndex].m_value);
}
//------------------------------------------------------------------------------

Qt::ItemFlags DebugProperties::flags(const QModelIndex& index) const
{
	Qt::ItemFlags flag = Qt::ItemIsSelectable;
	if(index.row() >= NB_CLASS_INFO_ROWS && !m_vMembersVar.at(m_vDisplayedIndex.at(index.row() - NB_CLASS_INFO_ROWS)).m_bReadOnly &&
		index.column() == 1)
		flag = Qt::ItemIsEnabled | Qt::ItemIsEditable;

	return flag;
}

void DebugProperties::applyFilter()
{
	m_vDisplayedIndex.clear();
	for (int i = 0; i < m_vMembersVar.size(); ++i)
	{
		if (m_regExp.match(m_vMembersVar.at(i).m_sName).hasMatch())
			m_vDisplayedIndex.emplace_back(i);
	}
}

void DebugProperties::filter(const QRegularExpression& a_filter)
{
	m_regExp = a_filter;
	beginResetModel();
	applyFilter();
	endResetModel();
}

void DebugProperties::setup(const Debugger::ClassInfo& a_info, const int64_t& a_uid, const VarList& a_vData)
{	
	m_info = a_info;
	m_uid = a_uid;
	beginResetModel();
	m_vDisplayedIndex.clear();
	m_vMembersVar.clear();
	m_vMembersVar = a_vData;
	applyFilter();
	endResetModel();
}

QModelIndex DebugProperties::index(int row, int column, const QModelIndex& parent) const
{
	return createIndex(row, column);
}

QModelIndex DebugProperties::parent(const QModelIndex& child) const
{
	return QModelIndex();
}

int DebugProperties::rowCount(const QModelIndex& parent) const
{
	return static_cast<int>(m_vDisplayedIndex.size() + NB_CLASS_INFO_ROWS);
}

int DebugProperties::columnCount(const QModelIndex& parent) const
{
	return 2;
}

QVariant DebugProperties::data(const QModelIndex& index, int role) const
{
	QVariant var;

	int iRow = index.row();
	int iVarOffset = iRow - NB_CLASS_INFO_ROWS;

	
	if (index.column() == 0)
	{
		switch (role)
		{
		case Qt::DisplayRole:
			switch (iRow)
			{
			case 0:
				var = "Class name";
				break;

			case 1:
				var = "Inheritance";
				break;

			case 2:
				var = "Size";
				break;

			default:
				var = m_vMembersVar.at(m_vDisplayedIndex.at(iVarOffset)).m_sName;
				break;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		switch (role)
		{
		case Qt::DisplayRole:
			switch (iRow)
			{
			case 0:
				var = QString::fromStdString(m_info.m_className);
				break;

			case 1:
				var = QString::fromStdString(m_info.m_inheritance);
				break;

			case 2:
				var = QString("%1 o").arg(m_info.m_classSize);
				break;

			default:
				var = m_vMembersVar.at(m_vDisplayedIndex.at(iVarOffset)).m_value;
				break;
			}
			
			break;

		case Qt::ForegroundRole:
			if(iRow >= NB_CLASS_INFO_ROWS)
				var = variableForeground(iVarOffset);
			break;

		case Qt::ToolTipRole:
			if (iRow >= NB_CLASS_INFO_ROWS)
				var = variableTooltip(iVarOffset);
			break;
		}
	}
	return var;
}

QVariant DebugProperties::variableForeground(const int a_index)const
{
	QVariant var;
	switch (m_vMembersVar.at(m_vDisplayedIndex.at(a_index)).m_type)
	{
	case Debugger::EVarType::type_bool:
		var = QColor(0, 237, 255);
		break;

	case Debugger::EVarType::type_short:
		var = QColor(0, 147, 41);
		break;

	case Debugger::EVarType::type_ushort:
		var = QColor(38, 147, 0);
		break;

	case Debugger::EVarType::type_int:
		var = QColor(255, 170, 0);
		break;

	case Debugger::EVarType::type_uint:
		var = QColor(255, 100, 0);
		break;

	case Debugger::EVarType::type_float:
		var = QColor(255, 0, 0);
		break;

	case Debugger::EVarType::type_double:
		var = QColor(255, 0, 178);
		break;

	case Debugger::EVarType::type_string:
		var = QColor(0, 0, 255);
		break;

	default:
		break;
	}
	return var;
}

QVariant DebugProperties::variableTooltip(const int a_index)const
{
	QVariant var;
	switch (m_vMembersVar.at(m_vDisplayedIndex.at(a_index)).m_type)
	{
	case Debugger::EVarType::type_bool:
		var = "Bool";
		break;

	case Debugger::EVarType::type_short:
		var = "Short";
		break;

	case Debugger::EVarType::type_ushort:
		var = "Unsigned short";
		break;

	case Debugger::EVarType::type_int:
		var = "Int";
		break;

	case Debugger::EVarType::type_uint:
		var = "Unsigned int";
		break;

	case Debugger::EVarType::type_float:
		var = "Float";
		break;

	case Debugger::EVarType::type_double:
		var = "Double";
		break;

	case Debugger::EVarType::type_string:
		var = "String";
		break;

	default:
		break;
	}
	return var;
}

QVariant DebugProperties::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant var;
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0:
			var = "Property";
			break;
		case 1:
			var = "Value";
			break;
		default:
			break;
		}

	}
	return var;
}
