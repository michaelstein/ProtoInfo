#include "ProtoTypeModelFileItem.h"
#include "ProtoTypeModelMessageItem.h"
#include "ProtoTypeModelRootItem.h"

ProtoTypeModelFileItem::ProtoTypeModelFileItem(const google::protobuf::FileDescriptor* fileDescriptor, ProtoTypeModelRootItem* parent)
	: _item(fileDescriptor)
	, _parent(parent)
{
	for (auto i = 0; i < fileDescriptor->message_type_count(); i++) {
		const auto* descriptor = fileDescriptor->message_type(i);
		appendChild(new ProtoTypeModelMessageItem(descriptor, this));
	}
}

ProtoTypeModelFileItem::~ProtoTypeModelFileItem()
{
	qDeleteAll(_children);
}

void ProtoTypeModelFileItem::appendChild(ProtoTypeModelItem* child)
{
	auto* message = reinterpret_cast<ProtoTypeModelMessageItem*>(child);
	if (message)
		_children.append(message);
}

ProtoTypeModelItem* ProtoTypeModelFileItem::child(int row)
{
	return _children.value(row);
}

int ProtoTypeModelFileItem::childCount() const
{
	return _children.count();
}

int ProtoTypeModelFileItem::columnCount() const
{
	return 1;
}

QVariant ProtoTypeModelFileItem::data(int column) const
{
	return QString::fromStdString(_item->name());
}

int ProtoTypeModelFileItem::row() const
{
	return 0;
}

ProtoTypeModelItem* ProtoTypeModelFileItem::parentItem()
{
	return nullptr;
}

int ProtoTypeModelFileItem::indexOf(ProtoTypeModelItem* item) const
{
	auto* childItem = reinterpret_cast<ProtoTypeModelMessageItem*>(item);
	if (childItem)
		return _children.indexOf(childItem);
	return -1;
}
