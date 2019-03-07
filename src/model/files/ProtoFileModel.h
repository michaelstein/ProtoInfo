#pragma once
#include <memory>
#include <string>
#include <QtCore/QAbstractListModel>

class ProtoFileModel : public QAbstractListModel
{
	Q_OBJECT

public:
	ProtoFileModel(QMap<QString, QVariant>& values, const QString& directoryPath, QObject *parent = nullptr);
	~ProtoFileModel() override;

	void setAllChecked(bool checked = true);

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	Qt::ItemFlags flags (const QModelIndex& index) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex& index, const QVariant& value, int role) override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
	class Private;
	std::unique_ptr<Private> d;
};
