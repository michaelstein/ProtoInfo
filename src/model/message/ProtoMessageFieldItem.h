#pragma once
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "QtCore/QList"
#include "QtCore/QVariant"

class ProtoMessageFieldItem
{
public:
	ProtoMessageFieldItem(const google::protobuf::FieldDescriptor* descriptor, const google::protobuf::Message* message, const int index = -1, ProtoMessageFieldItem* parent = nullptr);
	virtual ~ProtoMessageFieldItem();

	virtual void appendChild(ProtoMessageFieldItem* child);
	virtual ProtoMessageFieldItem* child(int row);
	virtual int childCount() const;
	virtual int columnCount() const;
	virtual QVariant data(int column) const;
	virtual int row() const;
	virtual ProtoMessageFieldItem* parentItem();
	virtual int indexOf(ProtoMessageFieldItem* item) const;

	static ProtoMessageFieldItem* createRoot(const google::protobuf::Descriptor* descriptor, const google::protobuf::Message* message);

public:
	const google::protobuf::FieldDescriptor* _descriptor;
	const google::protobuf::Message* _message;
	int _index;

	ProtoMessageFieldItem* _parent;
	QList<ProtoMessageFieldItem*> _children;

protected:
	ProtoMessageFieldItem();
};
