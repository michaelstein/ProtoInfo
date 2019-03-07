#include "ProtoTypeModel.h"
#include "ProtoTypeModelRootItem.h"

class ProtoTypeModel::Private
{
public:
	Private(ProtoTypeModel* owner)
		: q(owner)
	{}

	ProtoTypeModel* q;
	std::unique_ptr<ProtoTypeModelRootItem> root;
	const std::vector<const google::protobuf::FileDescriptor*>* values;
};

ProtoTypeModel::ProtoTypeModel(const std::vector<const google::protobuf::FileDescriptor*>& values, QObject* parent)
	: QAbstractItemModel(parent)
	, d(std::make_unique<Private>(this))
{
	d->values = &values;
	d->root = std::make_unique<ProtoTypeModelRootItem>(values);
}

ProtoTypeModel::~ProtoTypeModel() = default;

Qt::ItemFlags ProtoTypeModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
		return {};
	return QAbstractItemModel::flags(index);
}

int ProtoTypeModel::rowCount(const QModelIndex& parent) const
{
	ProtoTypeModelItem* parentItem = nullptr;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = d->root.get();
	else
		parentItem = static_cast<ProtoTypeModelItem*>(parent.internalPointer());

	return parentItem->childCount();
}

int ProtoTypeModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid())
		return static_cast<ProtoTypeModelItem*>(parent.internalPointer())->columnCount();
	else
		return d->root->columnCount();
}

QVariant ProtoTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return d->root->data(section);
	return {};
}

QVariant ProtoTypeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return {};

	if (role == Qt::DisplayRole)
	{
		auto* item = static_cast<ProtoTypeModelItem*>(index.internalPointer());
		return item->data(index.column());
	}

	return {};
}

QModelIndex ProtoTypeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
		return {};

	ProtoTypeModelItem *parentItem = nullptr;

	if (!parent.isValid())
		parentItem = d->root.get();
	else
		parentItem = static_cast<ProtoTypeModelItem*>(parent.internalPointer());

	auto* childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return {};
}

QModelIndex ProtoTypeModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return {};

	auto *childItem = static_cast<ProtoTypeModelItem*>(index.internalPointer());
	auto *parentItem = childItem->parentItem();

	if (!parentItem || parentItem == d->root.get())
		return {};

	return createIndex(parentItem->row(), 0, parentItem);
}

void ProtoTypeModel::updateAll()
{
	//emit dataChanged(index(0, 0), index(d->values->size() - 1, 0), {Qt::DisplayRole});
}

ProtoTypeModelMessageItem*ProtoTypeModel::messageItemByIndex(const QModelIndex& index) const
{
	auto* item = static_cast<ProtoTypeModelItem*>(index.internalPointer());
	if (!item)
		return nullptr;

	auto* messageItem = dynamic_cast<ProtoTypeModelMessageItem*>(item);
	if (messageItem)
		return messageItem;

	return nullptr;
}
