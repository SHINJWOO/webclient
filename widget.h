#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QSslSocket>
#include <QFile>
#include <QDataStream>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //QTcpSocket socket_;
    QSslSocket socket_;

public slots:
    void doConnected();
    void doDisconnected();
    void doReadyRead();

private slots:
    void on_pbLoad_clicked();
    void on_pbSave_clicked();

private slots:
    void onSocketStateChanged(QAbstractSocket::SocketState state);

private slots:
    void on_pbConnect_clicked();

    void on_pbDisconnect_clicked();

    void on_Send_clicked();

    void on_cbUseSSL_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
