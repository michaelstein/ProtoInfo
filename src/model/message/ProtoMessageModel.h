#pragma once
#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/message.h"
#include <QtCore/QAbstractItemModel>
#include "ProtoMessageFieldItem.h"
#include <memory>
#include <vector>

class ProtoMessageModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	ProtoMessageModel(const google::protobuf::Descriptor* descriptor, const google::protobuf::Message* message, QObject* parent = nullptr);
	~ProtoMessageModel() override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &index) const override;

	void updateAll();
	ProtoMessageFieldItem* messageItemByIndex(const QModelIndex &index) const;

private:
	class Private;
	std::unique_ptr<Private> d;
};
