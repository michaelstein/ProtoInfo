#include "ProtoMessageModel.h"

class ProtoMessageModel::Private
{
public:
	Private(ProtoMessageModel* owner)
		: q(owner)
	{}

	ProtoMessageModel* q;
	std::unique_ptr<ProtoMessageFieldItem> root;
};

ProtoMessageModel::ProtoMessageModel(const google::protobuf::Descriptor* descriptor, const google::protobuf::Message* message, QObject* parent)
	: QAbstractItemModel(parent)
	, d(std::make_unique<Private>(this))
{
	d->root.reset(ProtoMessageFieldItem::createRoot(descriptor, message));
}

ProtoMessageModel::~ProtoMessageModel() = default;

Qt::ItemFlags ProtoMessageModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
		return {};
	return QAbstractItemModel::flags(index);
}

int ProtoMessageModel::rowCount(const QModelIndex& parent) const
{
	ProtoMessageFieldItem* parentItem = nullptr;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = d->root.get();
	else
		parentItem = static_cast<ProtoMessageFieldItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int ProtoMessageModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid())
		return static_cast<ProtoMessageFieldItem*>(parent.internalPointer())->columnCount();
	else
		return d->root->columnCount();
}

QVariant ProtoMessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return d->root->data(section);
	return {};
}

QVariant ProtoMessageModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return {};

	if (role == Qt::DisplayRole)
	{
		auto* item = static_cast<ProtoMessageFieldItem*>(index.internalPointer());
		return item->data(index.column());
	}

	return {};
}

QModelIndex ProtoMessageModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return {};

	ProtoMessageFieldItem *parentItem = nullptr;

	if (!parent.isValid())
		parentItem = d->root.get();
	else
		parentItem = static_cast<ProtoMessageFieldItem*>(parent.internalPointer());

	auto* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return {};
}

QModelIndex ProtoMessageModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return {};

	auto *childItem = static_cast<ProtoMessageFieldItem*>(index.internalPointer());
	auto *parentItem = childItem->parentItem();

	if (!parentItem || parentItem == d->root.get())
		return {};

	return createIndex(parentItem->row(), 0, parentItem);
}

void ProtoMessageModel::updateAll()
{
	//emit dataChanged(index(0, 0), index(d->values->size() - 1, 0), {Qt::DisplayRole});
}

ProtoMessageFieldItem* ProtoMessageModel::messageItemByIndex(const QModelIndex& index) const
{
	return static_cast<ProtoMessageFieldItem*>(index.internalPointer());
}
