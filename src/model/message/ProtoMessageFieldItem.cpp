#include "ProtoMessageFieldItem.h"
#include "google/protobuf/text_format.h"
#include "humblelogging/api.h"

HUMBLE_LOGGER(L, "ProtoMessageFieldItem");

static QString fieldValue(const google::protobuf::Message* message, const google::protobuf::FieldDescriptor* descriptor, const int index = -1)
{
	auto* reflection = message->GetReflection();
	switch(descriptor->type()) {
		case google::protobuf::FieldDescriptor::TYPE_DOUBLE: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedDouble(*message, descriptor, index));
			else
				return QString::number(reflection->GetDouble(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_FLOAT: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedDouble(*message, descriptor, index));
			else
				return QString::number(reflection->GetDouble(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_INT64: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedInt64(*message, descriptor, index));
			else
				return QString::number(reflection->GetInt64(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_UINT64: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedUInt64(*message, descriptor, index));
			else
				return QString::number(reflection->GetUInt64(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_INT32: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedInt32(*message, descriptor, index));
			else
				return QString::number(reflection->GetInt32(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_FIXED64: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedUInt64(*message, descriptor, index));
			else
				return QString::number(reflection->GetUInt64(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_FIXED32: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedUInt32(*message, descriptor, index));
			else
				return QString::number(reflection->GetUInt32(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_BOOL: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedBool(*message, descriptor, index));
			else
				return reflection->GetBool(*message, descriptor) ? "true" : "false";
		}
		case google::protobuf::FieldDescriptor::TYPE_STRING: {
			if(index > 0)
				return QString::fromStdString(reflection->GetRepeatedString(*message, descriptor, index));
			else
				return QString::fromStdString(reflection->GetString(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_GROUP: {
			break;
		}
		case google::protobuf::FieldDescriptor::TYPE_MESSAGE: {
			if(index > 0) {
				break;
			} else {
				const auto* field = descriptor->message_type();
				return QString::fromStdString(field->name());
			}
		}
		case google::protobuf::FieldDescriptor::TYPE_BYTES: {
			const auto valueStr = reflection->GetString(*message, descriptor);
			QByteArray value(valueStr.data(), valueStr.size());
			return QString(value.toHex(' '));
		}
		case google::protobuf::FieldDescriptor::TYPE_UINT32: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedUInt32(*message, descriptor, index));
			else
				return QString::number(reflection->GetUInt32(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_ENUM: {
			if(index > 0)
				return QString::fromStdString(reflection->GetRepeatedEnum(*message, descriptor, index)->name());
			else
				return QString::fromStdString(reflection->GetEnum(*message, descriptor)->name());
		}
		case google::protobuf::FieldDescriptor::TYPE_SFIXED32: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedInt32(*message, descriptor, index));
			else
				return QString::number(reflection->GetInt32(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_SFIXED64: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedInt64(*message, descriptor, index));
			else
				return QString::number(reflection->GetInt64(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_SINT32: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedInt32(*message, descriptor, index));
			else
				return QString::number(reflection->GetInt32(*message, descriptor));
		}
		case google::protobuf::FieldDescriptor::TYPE_SINT64: {
			if(index > 0)
				return QString::number(reflection->GetRepeatedInt64(*message, descriptor, index));
			else
				return QString::number(reflection->GetInt64(*message, descriptor));
		}
	}

	return {};
}

ProtoMessageFieldItem::ProtoMessageFieldItem()
	: _descriptor(nullptr)
	, _message(nullptr)
	, _index(-1)
	, _parent(nullptr)
{
}

ProtoMessageFieldItem::ProtoMessageFieldItem(const google::protobuf::FieldDescriptor* descriptor, const google::protobuf::Message* message, const int index, ProtoMessageFieldItem* parent)
	: _descriptor(descriptor)
	, _message(message)
	, _index(index)
	, _parent(parent)
{
	if (_descriptor->is_repeated() && index < 0) {
		auto* reflection = message->GetReflection();
		const auto numFields = reflection->FieldSize(*message, descriptor);
		for (auto i = 0; i < numFields; i++) {
			auto* child = new ProtoMessageFieldItem(descriptor, message, i, this);
			appendChild(child);
		}
	} else if (_descriptor->type() == google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
		auto* reflection = message->GetReflection();

		const google::protobuf::Message* childMessage = nullptr;
		try {
			childMessage = &reflection->GetMessage(*message, descriptor);
		} catch (const std::exception& e) {
			HL_ERROR(L, std::string(e.what()));
		}

		if (childMessage) {
			auto* childDescriptor = _descriptor->message_type();

			for (auto i = 0; i < childDescriptor->field_count(); i++) {
				auto* fieldDescriptor = childDescriptor->field(i);
				if (!fieldDescriptor)
					continue;

				auto* child = new ProtoMessageFieldItem(fieldDescriptor, childMessage, -1, this);
				appendChild(child);
			}
		}
	}
}

ProtoMessageFieldItem::~ProtoMessageFieldItem()
{
	qDeleteAll(_children);
}

void ProtoMessageFieldItem::appendChild(ProtoMessageFieldItem* child)
{
	if (child)
		_children.append(child);
}

ProtoMessageFieldItem*ProtoMessageFieldItem::child(int row)
{
	return _children.value(row);
}

int ProtoMessageFieldItem::childCount() const
{
	return _children.count();
}

int ProtoMessageFieldItem::columnCount() const
{
	return 2;
}

QVariant ProtoMessageFieldItem::data(int column) const
{
	if (!_descriptor) { // Header data.
		if (column == 0)
			return "Field name";
		if (column == 1)
			return "Field value";
	} else {
		if (column == 0) {
			return QString::fromStdString(_descriptor->name());
		} else if (column == 1) {
			try {
				return fieldValue(_message, _descriptor, _index);
			} catch (const std::exception& e) {
				return QString::fromLatin1(e.what());
			}
		}
	}

	return {};
}

int ProtoMessageFieldItem::row() const
{
	return _parent->indexOf(const_cast<ProtoMessageFieldItem*>(this));
}

ProtoMessageFieldItem* ProtoMessageFieldItem::parentItem()
{
	return _parent;
}

int ProtoMessageFieldItem::indexOf(ProtoMessageFieldItem* item) const
{
	if (item)
		return _children.indexOf(item);
	return -1;
}

ProtoMessageFieldItem* ProtoMessageFieldItem::createRoot(const google::protobuf::Descriptor* descriptor, const google::protobuf::Message* message)
{
	auto* ret = new ProtoMessageFieldItem();
	ret->_message = message;

	for (auto i = 0; i < descriptor->field_count(); i++) {
		auto* fieldDescriptor = descriptor->field(i);
		if (!fieldDescriptor)
			continue;

		auto* field = new ProtoMessageFieldItem(fieldDescriptor, message, -1, ret);
		ret->appendChild(field);
	}

	return ret;
}



































