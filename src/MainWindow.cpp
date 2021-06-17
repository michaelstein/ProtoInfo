#include "MainWindow.h"
#include "google/protobuf/compiler/importer.h"
#include "google/protobuf/dynamic_message.h"
#include "humblelogging/api.h"
#include "ui_MainWindow.h"
#include "model/type/ProtoTypeModel.h"
#include "model/message/ProtoMessageModel.h"
#include "QtCore/QByteArray"
#include "QtCore/QDebug"
#include "QtCore/QString"
#include "QtCore/QSettings"
#include "QtWidgets/QFileDialog"
#include "SettingsWidget.h"
#include <map>
#include <string>
#include <vector>

HUMBLE_LOGGER(L, "ProtoInfo");

class HumbleErrorCollector : public google::protobuf::compiler::MultiFileErrorCollector
{
	void AddError(const std::string& filename, int line, int column, const std::string& message) override
	{
		HL_ERROR(L, "Protobuf error in " + filename + " (" + std::to_string(line) + ":" + std::to_string(column) + "): " + message);
	}

	void AddWarning(const std::string& filename, int line, int column, const std::string& message) override
	{
		HL_WARN(L, "Protobuf warning in " + filename + " (" + std::to_string(line) + ":" + std::to_string(column) + "): " + message);
	}
};

class MainWindow::Private
{
public:
	Private(MainWindow* owner)
		: q(owner)
	{}

	MainWindow* q;
	Ui_MainWindow ui;
	std::unique_ptr<SettingsWidget> settingsWidget;

	std::vector<const google::protobuf::FileDescriptor*> descriptors;
	std::unique_ptr<ProtoTypeModel> typeModel;

	HumbleErrorCollector errorCollector;
	std::unique_ptr<google::protobuf::compiler::DiskSourceTree> sourceTree;
	std::unique_ptr<google::protobuf::compiler::Importer> importer;

	std::unique_ptr<google::protobuf::DynamicMessageFactory> messageFactory;
	std::unique_ptr<google::protobuf::Message> message;
	std::unique_ptr<ProtoMessageModel> messageModel;

	void loadProto()
	{
		// Clear views and models.
		ui.messageView->setModel(nullptr);
		ui.typeView->setModel(nullptr);
		messageModel.reset();
		typeModel.reset();

		// Reset loaded protocol buffers.
		message.reset();
		messageFactory.reset();
		descriptors.clear();
		importer.reset();
		sourceTree.reset();

		// Read settings.
		QSettings settings;
		const auto files = settings.value("proto/files").toMap();
		const auto directories = settings.value("proto/directories").toStringList();

		// Register directory roots with proto files.
		const auto protoRootDir = QStringLiteral("%1/protobuf").arg(QCoreApplication::applicationDirPath());
		sourceTree = std::make_unique<google::protobuf::compiler::DiskSourceTree>();
		sourceTree->MapPath("", QDir(protoRootDir).absolutePath().toStdString());
		for (const auto& path : directories)
			sourceTree->MapPath("", path.toStdString());

		importer = std::make_unique<google::protobuf::compiler::Importer>(sourceTree.get(), &errorCollector);
		for (const auto& file : files.keys())
		{
			if (!files.value(file).toBool())
				continue;

			auto* descriptor = importer->Import(file.toStdString());
			if (descriptor)
				descriptors.push_back(descriptor);
		}

		typeModel = std::make_unique<ProtoTypeModel>(descriptors);
		ui.typeView->setModel(typeModel.get());
	}

	void encode()
	{
	}

	void decode()
	{
		// Read input.
		const auto dataStr = ui.encodedEdit->toPlainText();
		QByteArray data;
		if (ui.base64Radio->isChecked())
			data = QByteArray::fromBase64(dataStr.toLatin1());
		else if (ui.hexRadio->isChecked())
			data = QByteArray::fromHex(dataStr.toLatin1());
		if (data.isEmpty())
			return;

		// Check message type selection.
		const auto index = ui.typeView->currentIndex();
		if (!index.isValid())
			return;

		// Retrieve message type information.
		auto* item = typeModel->messageItemByIndex(index);
		if (!item)
			return;

		// Create message prototype.
		if (!messageFactory)
			messageFactory = std::make_unique<google::protobuf::DynamicMessageFactory>();
		const auto* type = messageFactory->GetPrototype(item->_item);

		// Decode input to create actual message.
		message.reset(type->New());
		if (!message->ParseFromArray(data.data(), data.size()))
			return;

		// Create model.
		auto model = std::make_unique<ProtoMessageModel>(item->_item, message.get());
		ui.messageView->setModel(model.get());
		messageModel = std::move(model);
	}
};

MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent)
	, d(std::make_unique<Private>(this))
{
	d->ui.setupUi(this);
	d->loadProto();
}

MainWindow::~MainWindow()
{
	d->message.reset();
}

void MainWindow::on_openFileButton_clicked(bool checked)
{
	const auto filepath = QFileDialog::getOpenFileName(this);
	QFileInfo info(filepath);
	if (!info.exists() || !info.isFile())
		return;

	QFile file(filepath);
	if (!file.open(QIODevice::ReadOnly))
		return;

	const auto data = file.readAll();
	const auto hex = QString::fromLatin1(data.toHex(' '));

	d->ui.encodedEdit->setPlainText(hex);
	d->ui.hexRadio->toggle();
}

void MainWindow::on_decodeButton_clicked(bool checked)
{
	d->decode();
}

void MainWindow::on_settingsButton_clicked(bool checked)
{
	if (!d->settingsWidget)
	{
		d->settingsWidget = std::make_unique<SettingsWidget>();
		QObject::connect(d->settingsWidget.get(), &SettingsWidget::settingsChanged, this, &MainWindow::reloadProto);
	}
	d->settingsWidget->show();
}

void MainWindow::reloadProto()
{
	d->loadProto();
}
