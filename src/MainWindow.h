#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtWidgets/QWidget>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:
	void on_openFileButton_clicked(bool checked = false);
	void on_decodeButton_clicked(bool checked = false);
	void on_settingsButton_clicked(bool checked = false);

	void reloadProto();

private:
	class Private;
	std::unique_ptr<Private> d;

};
