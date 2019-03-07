#pragma once
#include <memory>
#include <QtCore/QObject>
#include <QtWidgets/QWidget>

class SettingsWidget : public QWidget
{
	Q_OBJECT

public:
	SettingsWidget(QWidget *parent = nullptr);
	~SettingsWidget();

public slots:
	void on_addDirectoryButton_clicked(bool checked = false);
	void on_removeDirectoryButton_clicked(bool checked = false);

	void on_checkFilesButton_clicked(bool checked = false);
	void on_uncheckFilesButton_clicked(bool checked = false);

	void on_buttonBox_accepted();
	void on_buttonBox_rejected();

	void on_directoryView_clicked(const QModelIndex& index);

signals:
	void settingsChanged();

private:
	class Private;
	std::unique_ptr<Private> d;
};
