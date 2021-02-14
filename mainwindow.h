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


struct struct_Slides1
{
    //QString name;
    QVector<QByteArray> slide1;
};










class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private:
    Ui::mainwindow *ui;



public:
    QString mainUrl;
    QString meetingID;
    QMap<QString,QString>  hexTounicode;
    QStringList matchs;

    QByteArray chats;
    QByteArray cursor;
    QByteArray metadata;
    QByteArray deskshare1;
    QByteArray webcams;
    QByteArray sourceSilde;
    QByteArray moving;




    QFile chat_file;
    QFile cursor_file;
    QFile metadata_file;
    QFile deskshare1_file;
    QFile webcams_file;
    QFile sourceSlide_file;
    QFile moving_file;
    QFile detailDesshare_file;
    QFile match_namefile;



    void start_program();
    void downloader();
    void add_hexTounicode();
    QByteArray convertTounicode(QByteArray);




public:

    struct struct_chat
    {
        int in;
        QString name;
        QString massage;
    };

    struct struct_moving
    {
        float in;
        float out;
        int number_slide;
        int number_seriSlide;// -1 ==deskshare   -2==logo
        int wigth;
        int height;
        int x;
        int y;
    };


    QVector<struct_Slides1> serial_Slide;
    QList<struct_chat> serial_Chat;
    QList<struct_moving> serial_Moving;
    QMap<QString,int> convertNameSlideTonumber;






    void player();
};
#endif // MAINWINDOW_H
