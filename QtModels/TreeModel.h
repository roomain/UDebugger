#pragma once
#include <memory>
#include <QAbstractItemModel>
#include "qtmodels_global.h"
class ITreeNode;

class QTMODELS_EXPORT TreeModel : public QAbstractItemModel
{
	Q_OBJECT
protected:
	std::shared_ptr<ITreeNode> m_pRoot;

public:
	TreeModel();
	virtual ~TreeModel()override = default;
	const std::shared_ptr<ITreeNode>& root()const { return m_pRoot; }
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex& child) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
};

