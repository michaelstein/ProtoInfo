#pragma once
#include "ProtoTypeModelItem.h"
#include "google/protobuf/descriptor.h"
#include "QtCore/QList"

class ProtoTypeModelRootItem;
class ProtoTypeModelMessageItem;

class ProtoTypeModelFileItem : public ProtoTypeModelItem
{
public:
	ProtoTypeModelFileItem(const google::protobuf::FileDescriptor* descriptor, ProtoTypeModelRootItem* parent);
	~ProtoTypeModelFileItem() override;

	void appendChild(ProtoTypeModelItem* child) override;
	ProtoTypeModelItem* child(int row) override;
	int childCount() const override;
	int columnCount() const override;
	QVariant data(int column) const override;
	int row() const override;
	ProtoTypeModelItem* parentItem() override;
	int indexOf(ProtoTypeModelItem* item) const override;

	const google::protobuf::FileDescriptor* _item;
	QList<ProtoTypeModelMessageItem*> _children;
	ProtoTypeModelRootItem* _parent;
};
