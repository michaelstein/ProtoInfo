#include "ProtoTypeModelMessageItem.h"
#include "ProtoTypeModelFileItem.h"

ProtoTypeModelMessageItem::ProtoTypeModelMessageItem(const google::protobuf::Descriptor* descriptor, ProtoTypeModelFileItem* parent)
	: _item(descriptor)
	, _parent(parent)
{
}

ProtoTypeModelMessageItem::~ProtoTypeModelMessageItem() = default;

void ProtoTypeModelMessageItem::appendChild(ProtoTypeModelItem* child)
{
}

ProtoTypeModelItem* ProtoTypeModelMessageItem::child(int row)
{
	return nullptr;
}

int ProtoTypeModelMessageItem::childCount() const
{
	return 0;
}

int ProtoTypeModelMessageItem::columnCount() const
{
	return 1;
}

QVariant ProtoTypeModelMessageItem::data(int column) const
{
	return QString::fromStdString(_item->name());
}

int ProtoTypeModelMessageItem::row() const
{
	return _parent->indexOf(const_cast<ProtoTypeModelMessageItem*>(this));
}

ProtoTypeModelItem* ProtoTypeModelMessageItem::parentItem()
{
	return _parent;
}

int ProtoTypeModelMessageItem::indexOf(ProtoTypeModelItem* item) const
{
	return -1;
}
