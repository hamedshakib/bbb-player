#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qnetwork.h"
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkAccessManager"
#include "QtNetwork/QNetworkDatagram"
#include "QtNetwork/QNetworkRequest"
#include "QtNetwork/qsslsocket.h"
#include "qurl.h"
#include "QUrlQuery"
#include "qjsonobject.h"
#include "qjsondocument.h"
#include "QJsonValue"

#include "qdebug.h"
#include "iostream"
#include "string"
#include "QString"
#include "QMap"
#include "QFile"
#include "QRegExp"
#include "QDir"


QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private:
    Ui::mainwindow *ui;



public:
    QSslSocket *socket1;
    QString mainUrl;
    QString meetingID;
    QMap<QString,QString>  hexTounicode;

    QByteArray chats;
    QByteArray cursor;
    QByteArray metadata;
    QByteArray deskshare1;
    QByteArray webcams;
    QByteArray sourceSilde;



    QFile chat_file;
    QFile cursor_file;
    QFile metadata_file;
    QFile deskshare1_file;
    QFile webcams_file;
    QFile sourceSlide_file;



    void start_program();
    void readSocket();
    void downloader();
    void add_hexTounicode();
    QByteArray convertTounicode(QByteArray);
};
#endif // MAINWINDOW_H
