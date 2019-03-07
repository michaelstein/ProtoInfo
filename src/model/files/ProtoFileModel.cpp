#include "ProtoFileModel.h"
#include "QtCore/QDir"
#include "QtCore/QFile"
#include "QtCore/QMap"

class ProtoFileModel::Private
{
public:
	Private(ProtoFileModel* owner)
		: q(owner)
	{}

	ProtoFileModel* q;

	QMap<QString, QVariant>* values;
	QStringList files;
};

ProtoFileModel::ProtoFileModel(QMap<QString, QVariant>& values, const QString& directoryPath, QObject* parent)
	: QAbstractListModel(parent)
	, d(std::make_unique<Private>(this))
{
	d->values = &values;
	QDir dir(directoryPath);
	if (dir.exists())
	{
		auto files = dir.entryInfoList();
		for (const auto& info : files)
		{
			const auto name = info.fileName();
			if (info.isFile() && name.endsWith(".proto"))
				d->files.append(name);
		}
	}
}

ProtoFileModel::~ProtoFileModel()
{
}

void ProtoFileModel::setAllChecked(bool checked)
{
	for (const auto& file : d->files)
		d->values->insert(file, checked);
}

int ProtoFileModel::rowCount(const QModelIndex& parent) const
{
	return d->files.count();
}

Qt::ItemFlags ProtoFileModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
	if (index.isValid())
		return defaultFlags | Qt::ItemIsUserCheckable;
	return defaultFlags;
}

QVariant ProtoFileModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		return d->files.at(index.row());
	}
	else if (role == Qt::CheckStateRole)
	{
		const auto key = d->files.at(index.row());
		return d->values->value(key).toBool() ? Qt::Checked : Qt::Unchecked;
	}

	return {};
}

bool ProtoFileModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if (!index.isValid() || role != Qt::CheckStateRole)
		return false;

	const auto key = d->files.at(index.row());
	d->values->insert(key, value == Qt::Checked);

	emit dataChanged(index, index);
	return true;
}

QVariant ProtoFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		return "Files";
	}

	return {};
}
