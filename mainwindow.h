#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "hamedmediaplayer.h"
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
//#include "QMediaPlayer"
//#include <QVideoWidget>
#include "qvideowidget.h"
#include "qthread.h"

#include "QListWidget"
#include "QListWidgetItem"
#include "QTextEdit"
#include "QSize"
#include "QScreen"
#include "QApplication"
#include "QDesktopWidget"
#include "QDialog"
#include "QLineEdit"
#include "QPushButton"
#include "QProgressBar"
#include "QGridLayout"
#include "QFileDialog"
#include "QMessageBox"


#define timer_mouse_time 100
#define timer_slide_time 100
#define timer_chat_time 1000



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
    //play
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
    struct struct_curcer
    {
        float timestamp;
        float x;
        float y;
    };




    enum{
        Mouse,Slide,desktopShare,Chats
    };


    QString address_downloaded;
    QVector<struct_Slides1> serial_Slide;
    QList<struct_chat> serial_Chat;
    QList<struct_moving> serial_Moving;
    QList<struct_curcer> serial_curcer;
    QMap<QString,int> convertNameSlideTonumber;
    HamedMediaPlayer *mediaPlayer1;
    HamedMediaPlayer *mediaPlayer2;

    QVideoWidget *videoWidget2;
    QVideoWidget *videoWidget1;
    QTimer *timer_mouse;
    QTimer *timer_slide;
    QTimer *timer_deskshare;
    QTimer *timer_chat;
    int now_mouse;
    int now_slide;
    int now_chat=0;
    QListWidget *widgetChats;

    QSize appSize;
    QDialog *download_Dialog;
    QGridLayout *dialog_layout;
    QProgressBar *progressBar;
    long long int All_size;
    long long int tempdownloaded=0;
    long long int downloaded_complited=0;
    bool played_player2=false;




    void player();
    void set_labelsize();

public slots:
    void checkNext(int type,int Number);
    void download_gui();
    void manageSizeDownload(qint64 bytesReceived, qint64 bytesTotal);
    void change_slider(float pos);
    void setplayAndPause();
private slots:
    void on_actionDownload_triggered();
    void on_actionOpen_File_triggered();
    void on_actionAbout_triggered();
};
#endif // MAINWINDOW_H
