#pragma once
#include <vector>
#include <qjsonobject.h>
#include <qabstractitemmodel.h>
#include "DebugType.h"
#include <qregularexpression.h>
#include "defines.h"
#include <QStyledItemDelegate>


class PropertiesDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index)const override;
signals:
	void sg_writeProperty(const qint64&, const int, const QVariant&)const;
};


class DebugProperties : public QAbstractItemModel
{
	friend class PropertiesDelegate;
	Q_OBJECT
private:
	qint64 m_uid;
	QRegularExpression m_regExp;
	VarList m_vMembersVar;
	std::vector<int> m_vDisplayedIndex;
	void applyFilter();
public:
	QAbstractItemModel::QAbstractItemModel;
	inline qint64 uid()const { return m_uid; }
	void filter(const QRegularExpression& a_filter);
	void setup(const qint64& a_uid, const VarList& a_vData);
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const final;
	QModelIndex parent(const QModelIndex& child) const final;
	int rowCount(const QModelIndex& parent = QModelIndex()) const final;
	int columnCount(const QModelIndex& parent = QModelIndex()) const final;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const final;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const final;
	Qt::ItemFlags flags(const QModelIndex& index) const final;

signals:
	void sg_write();
};

