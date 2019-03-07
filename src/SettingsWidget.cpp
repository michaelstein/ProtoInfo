#include "SettingsWidget.h"
#include "QtCore/QSettings"
#include "QtCore/QStringListModel"
#include "QtWidgets/QFileDialog"
#include "model/files/ProtoFileModel.h"
#include "ui_SettingsWidget.h"

class SettingsWidget::Private
{
public:
	Private(SettingsWidget* owner)
		: q(owner)
	{}

	SettingsWidget* q;
	Ui_SettingsWidget ui;

	std::unique_ptr<QStringListModel> directoryModel;
	QStringList directories;

	std::unique_ptr<ProtoFileModel> fileModel;
	QMap<QString, QVariant> files;
};

SettingsWidget::SettingsWidget(QWidget* parent)
	: QWidget(parent)
	, d(std::make_unique<Private>(this))
{
	d->ui.setupUi(this);

	QSettings settings;
	d->files = settings.value("proto/files").toMap();
	d->directories = settings.value("proto/directories").toStringList();
	d->directoryModel = std::make_unique<QStringListModel>(d->directories);
	d->ui.directoryView->setModel(d->directoryModel.get());
}

SettingsWidget::~SettingsWidget()
{
}

void SettingsWidget::on_addDirectoryButton_clicked(bool checked)
{
	const auto dirPath = QFileDialog::getExistingDirectory(this);
	d->directories.append(dirPath);
	d->directoryModel->setStringList(d->directories);

	QDir dir(dirPath);
	if (dir.exists())
	{
		auto files = dir.entryInfoList();
		for (const auto& info : files)
		{
			const auto name = info.fileName();
			if (info.isFile() && name.endsWith(".proto") && !d->files.contains(name))
				d->files.insert(name, true);
		}
	}
}

void SettingsWidget::on_removeDirectoryButton_clicked(bool checked)
{
	const auto index = d->ui.directoryView->currentIndex();
	d->directories.removeAt(index.row());
	d->directoryModel->setStringList(d->directories);
}

void SettingsWidget::on_checkFilesButton_clicked(bool checked)
{
	if (d->fileModel)
		d->fileModel->setAllChecked(true);
}

void SettingsWidget::on_uncheckFilesButton_clicked(bool checked)
{
	if (d->fileModel)
		d->fileModel->setAllChecked(false);
}

void SettingsWidget::on_buttonBox_accepted()
{
	QSettings settings;
	settings.setValue("proto/directories", d->directories);
	settings.setValue("proto/files", d->files);
	emit settingsChanged();
	close();
}

void SettingsWidget::on_buttonBox_rejected()
{
	close();
}

void SettingsWidget::on_directoryView_clicked(const QModelIndex& index)
{
	const auto dirPath = d->directoryModel->data(index).toString();
	auto fileModel = std::make_unique<ProtoFileModel>(d->files, dirPath);
	d->ui.fileView->setModel(fileModel.get());
	d->fileModel = std::move(fileModel);
}
