#pragma once
#include "ProtoTypeModelItem.h"
#include "google/protobuf/descriptor.h"
#include <vector>

class ProtoTypeModelFileItem;

class ProtoTypeModelRootItem : public ProtoTypeModelItem
{
public:
	ProtoTypeModelRootItem(const std::vector<const google::protobuf::FileDescriptor*>& descriptors);
	~ProtoTypeModelRootItem() override;

	void appendChild(ProtoTypeModelItem* child) override;
	ProtoTypeModelItem* child(int row) override;
	int childCount() const override;
	int columnCount() const override;
	QVariant data(int column) const override;
	int row() const override;
	ProtoTypeModelItem* parentItem() override;
	int indexOf(ProtoTypeModelItem* item) const override;

	QList<ProtoTypeModelFileItem*> _children;
};
