#pragma once
#include "google/protobuf/compiler/importer.h"
#include <QtCore/QAbstractItemModel>
#include "ProtoTypeModelMessageItem.h"
#include <memory>
#include <vector>

class ProtoTypeModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	ProtoTypeModel(const std::vector<const google::protobuf::FileDescriptor*>& values, QObject* parent = nullptr);
	~ProtoTypeModel() override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;

	void updateAll();
	ProtoTypeModelMessageItem* messageItemByIndex(const QModelIndex &index) const;

private:
	class Private;
	std::unique_ptr<Private> d;
};
