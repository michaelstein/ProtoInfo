#pragma once
#include "QtCore/QVariant"

class ProtoTypeModelItem
{
public:
	virtual ~ProtoTypeModelItem();
	virtual void appendChild(ProtoTypeModelItem* child) = 0;
	virtual ProtoTypeModelItem* child(int row) = 0;
	virtual int childCount() const = 0;
	virtual int columnCount() const = 0;
	virtual QVariant data(int column) const = 0;
	virtual int row() const = 0;
	virtual ProtoTypeModelItem* parentItem() = 0;
	virtual int indexOf(ProtoTypeModelItem* item) const = 0;
};
