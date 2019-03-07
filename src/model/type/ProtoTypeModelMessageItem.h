#pragma once
#include "ProtoTypeModelItem.h"
#include "google/protobuf/descriptor.h"

class ProtoTypeModelFileItem;

class ProtoTypeModelMessageItem : public ProtoTypeModelItem
{
public:
	ProtoTypeModelMessageItem(const google::protobuf::Descriptor* descriptor, ProtoTypeModelFileItem* parent);
	~ProtoTypeModelMessageItem() override;

	void appendChild(ProtoTypeModelItem* child) override;
	ProtoTypeModelItem* child(int row) override;
	int childCount() const override;
	int columnCount() const override;
	QVariant data(int column) const override;
	int row() const override;
	ProtoTypeModelItem* parentItem() override;
	int indexOf(ProtoTypeModelItem* item) const override;

	const google::protobuf::Descriptor* _item;
	ProtoTypeModelFileItem* _parent;
};
