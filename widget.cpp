#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDataStream>
#include <QCheckBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //socket_.connected();
    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_, &QIODevice::readyRead, this, &Widget::doReadyRead);
    QObject::connect(&socket_, &QAbstractSocket::stateChanged, this, &Widget::onSocketStateChanged);
    //QTcpSocket OK
    QObject::connect(ui->pbLoad, &QPushButton::clicked, this, &Widget::on_pbLoad_clicked);
    QObject::connect(ui->pbSave, &QPushButton::clicked, this, &Widget::on_pbSave_clicked);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::doConnected(){
    QString msg = "connected\r\n";
    ui->pteMessage->insertPlainText(msg);
}

void Widget::doDisconnected(){
    QString msg = "disconnected\r\n";
    ui->pteMessage->insertPlainText(msg);
}

void Widget::doReadyRead(){
    QString msg = socket_.readAll();
    ui->pteMessage->insertPlainText(msg);
}

void Widget::on_pbConnect_clicked()
{
//    socket_.connectToHost(ui->leHost->text(),ui->lePort->text().toUShort());
    socket_.connectToHostEncrypted(ui->leHost->text(),ui->lePort->text().toUShort());
}


void Widget::on_pbDisconnect_clicked()
{
    socket_.disconnectFromHost();
}

void Widget::on_Send_clicked()
{
    socket_.write(ui->pteSend->toPlainText().toUtf8());
}

void Widget::on_cbUseSSL_checkStateChanged(const Qt::CheckState &arg1) {
    if (arg1 == Qt::Checked) {
        // SSL을 사용하도록 설정
        // 예: SSL 관련 UI 요소를 활성화
        ui->pteMessage->insertPlainText("SSL 사용이 활성화되었습니다.\n");
    } else {
        // SSL을 사용하지 않도록 설정
        // 예: SSL 관련 UI 요소를 비활성화
        ui->pteMessage->insertPlainText("SSL 사용이 비활성화되었습니다.\n");
    }
}

void Widget::on_pbLoad_clicked()
{
    QFile file("uistate.dat");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);

        QRect geometry;
        in >> geometry;
        this->setGeometry(geometry);

        QString host, message, send;
        quint16 port;
        in >> host >> port >> message >> send;

        ui->leHost->setText(host);
        ui->lePort->setText(QString::number(port));
        ui->pteMessage->setPlainText(message);
        ui->pteSend->setPlainText(send);

        file.close();
    }
}

void Widget::on_pbSave_clicked()
{
    QFile file("uistate.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out << this->geometry();
        out << ui->leHost->text()
            << ui->lePort->text().toUShort()
            << ui->pteMessage->toPlainText()
            << ui->pteSend->toPlainText();

        file.close();
    }
}

void Widget::onSocketStateChanged(QAbstractSocket::SocketState state) {
    bool isConnected = (state == QAbstractSocket::ConnectedState);

    ui->pbConnect->setEnabled(!isConnected);
    ui->pbDisconnect->setEnabled(isConnected);
    ui->Send->setEnabled(isConnected);

    ui->pbSave->setEnabled(!isConnected);  // Maybe you'd want to disable save during connection.
    ui->pbLoad->setEnabled(!isConnected);  // Maybe you'd want to disable load during connection.
}

