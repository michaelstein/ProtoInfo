#include "ProtoTypeModelRootItem.h"
#include "ProtoTypeModelFileItem.h"

ProtoTypeModelRootItem::ProtoTypeModelRootItem(const std::vector<const google::protobuf::FileDescriptor*>& descriptors)
{
	for (const auto* descriptor : descriptors)
		appendChild(new ProtoTypeModelFileItem(descriptor, this));
}

ProtoTypeModelRootItem::~ProtoTypeModelRootItem()
{
	qDeleteAll(_children);
}

void ProtoTypeModelRootItem::appendChild(ProtoTypeModelItem* child)
{
	auto* message = reinterpret_cast<ProtoTypeModelFileItem*>(child);
	if (message)
		_children.append(message);
}

ProtoTypeModelItem* ProtoTypeModelRootItem::child(int row)
{
	return _children.value(row);
}

int ProtoTypeModelRootItem::childCount() const
{
	return _children.count();
}

int ProtoTypeModelRootItem::columnCount() const
{
	return 1;
}

QVariant ProtoTypeModelRootItem::data(int column) const
{
	return "Header";
}

int ProtoTypeModelRootItem::row() const
{
	return 0;
}

ProtoTypeModelItem* ProtoTypeModelRootItem::parentItem()
{
	return nullptr;
}

int ProtoTypeModelRootItem::indexOf(ProtoTypeModelItem* item) const
{
	auto* childItem = reinterpret_cast<ProtoTypeModelFileItem*>(item);
	if (childItem)
		return _children.indexOf(childItem);
	return -1;
}
