#include "conftool_import.h"
#include "ui_conftool_import.h"

conftool_import::conftool_import(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::conftool_import)
{
    ui->setupUi(this);
    ui->bOK->button(QDialogButtonBox::Ok)->setText("Import");
    connect(ui->tfConfigName,SIGNAL(textEdited(QString)),this,SLOT(tfConfigNameModified()));
    this->populateLvConfiguration();
}

conftool_import::~conftool_import()
{
    delete ui;
}

void conftool_import::populateLvConfiguration(){

    conftoolpy.start("conftool.py",QStringList()<<"getListOfAvailableRunConfigurations");
    conftoolpy.waitForFinished();
    QByteArray byte_status = conftoolpy.readAll();
    QTextCodec* codec;
    QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("\n",QString::KeepEmptyParts);
    qDebug()<<daq_string;
    QStringListModel* model = new QStringListModel(this);
    model->setStringList(daq_string);
    ui->lvConfigurationList->setModel(model);
}

void conftool_import::tfConfigNameModified(){

    QString comboText = ui->tfConfigName->text();
    conftoolpy.start("conftool.py",QStringList()<<"getListOfAvailableRunConfigurations"<<comboText);
    conftoolpy.waitForFinished();
    QByteArray byte_status = conftoolpy.readAll();
    QTextCodec* codec;
    QStringList daq_string = codec->codecForMib(106)->toUnicode(byte_status).split("\n",QString::KeepEmptyParts);
    QStringListModel* model = new QStringListModel(this);
    model->setStringList(daq_string);
    ui->lvConfigurationList->setModel(model);

}
