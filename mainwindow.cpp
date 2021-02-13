#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);

    start_program();
}

mainwindow::~mainwindow()
{
    delete ui;
}

void mainwindow::start_program()
{

    add_hexTounicode();
    mainUrl="https://blue.aut.ac.ir/playback/presentation/2.3/aafbd590b53508afa2497f062107ca4d7fef952b-1606726815618?meetingId=aafbd590b53508afa2497f062107ca4d7fef952b-1606726815618";
    //mainUrl="https://blue.aut.ac.ir/playback/presentation/2.3/4469cae04424e7c447dd19a015934c7b800c64c9-1602415836554?meetingId=4469cae04424e7c447dd19a015934c7b800c64c9-1602415836554";
    downloader();













}



















void mainwindow::readSocket()
{
    qDebug()<<"so";
    qDebug()<<socket1->readAll();
}

void mainwindow::downloader()
{
    meetingID=mainUrl.mid(mainUrl.indexOf("/2.3/",0)+5,mainUrl.length()-mainUrl.indexOf("?",0)-11);

    QNetworkRequest request1;
        QNetworkAccessManager *manager1 = new QNetworkAccessManager(this);
        QNetworkReply *replay1;
         QEventLoop eventLoop;
     QObject::connect(manager1, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);

     //make dir
     if(!QDir("Downloaded/"+meetingID).exists())
     {
       qDebug()<<"hil";
       QDir().mkdir("Downloaded/");
       QDir().mkdir("Downloaded/"+meetingID);
     }


    //chat
    QString ChatUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/slides_new.xml";
    //qDebug()<<ChatUrl;
    request1.setUrl(ChatUrl);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    chats = replay1->readAll();
    chats=convertTounicode(chats);
    QString namefileChat="Downloaded/"+meetingID+"/"+"chats.xml";
    chat_file.setFileName(namefileChat);
    if(chat_file.open(QIODevice::ReadWrite))
    {
       chat_file.write(chats);
       chat_file.close();
    }




    //cursor
    QString CursorUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/cursor.xml";
    request1.setUrl(CursorUrl);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    cursor = replay1->readAll();
    cursor=convertTounicode(cursor);
    QString namefileCursor="Downloaded/"+meetingID+"/"+"cursor.xml";
    cursor_file.setFileName(namefileCursor);
    if(cursor_file.open(QIODevice::ReadWrite))
    {
       cursor_file.write(cursor);
       cursor_file.close();
    }

    //metadata
    QString MetaData="https://blue.aut.ac.ir/presentation/"+meetingID+"/metadata.xml";
    request1.setUrl(MetaData);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    metadata = replay1->readAll();
    metadata=convertTounicode(metadata);
    QString namefileMetadata="Downloaded/"+meetingID+"/"+"metadata.xml";
    metadata_file.setFileName(namefileMetadata);
    if(metadata_file.open(QIODevice::ReadWrite))
    {
       metadata_file.write(metadata);
       metadata_file.close();
    }

    //desktopshare
    QString DeskshareUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/deskshare.xml";
    request1.setUrl(DeskshareUrl);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    QByteArray deskshare = replay1->readAll();


/*
    //deskshare_Asli just share screen
    QString DeskshareUrl1="https://blue.aut.ac.ir/presentation/"+meetingID+"/deskshare/deskshare.webm";
    request1.setUrl(DeskshareUrl1);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    deskshare1 = replay1->readAll();
    QString namefileDeskshare1="Downloaded/"+meetingID+"/"+"deskshare1.webm";
    deskshare1_file.setFileName(namefileDeskshare1);
    if(deskshare1_file.open(QIODevice::ReadWrite))
    {
       deskshare1_file.write(deskshare1);
       deskshare1_file.close();
    }
*/

    //webcams_Asli and sound
/*
    QString webcamsUrl1="https://blue.aut.ac.ir/presentation/"+meetingID+"/video/webcams.webm";
    request1.setUrl(webcamsUrl1);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    webcams = replay1->readAll();
    QString namefileWebcams="Downloaded/"+meetingID+"/"+"webcams.webm";
    webcams_file.setFileName(namefileWebcams);
    if(webcams_file.open(QIODevice::ReadWrite))
    {
       webcams_file.write(webcams);
       webcams_file.close();
    }
*/



    //slides
    QString SourceSlideUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/presentation_text.json";
    request1.setUrl(SourceSlideUrl);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    sourceSilde = replay1->readAll();
    //sourceSilde=convertTounicode(sourceSilde);
    QString namefileSourceSlide="Downloaded/"+meetingID+"/"+"presentation_text.json";
    sourceSlide_file.setFileName(namefileSourceSlide);
    if(sourceSlide_file.open(QIODevice::ReadWrite))
    {
       sourceSlide_file.write(sourceSilde);
       sourceSlide_file.close();
    }

    //QString
    QStringList address_ha;
    QString str123=sourceSilde;
    QRegExp re;
    re.setPatternSyntax(QRegExp::Wildcard);
    re.setPattern("\":{\"slide-1\":\"");

    int addad;
    while(re.indexIn(str123)!=-1)
    {
        int addad;
        for(int i=1;i<re.indexIn(str123);i++)
        {
            if(str123[re.indexIn(str123)-i]=='\"')
            {
               qDebug()<<"yes";
               addad=i;
               break;
            }

        }
        QString tempi=str123.mid(re.indexIn(str123)-addad+1,54);
        address_ha.append(tempi);
        str123.replace(re.indexIn(str123),tempi.length(),"");

    }


    //download slides
    for(int i=0;i<address_ha.count();i++)
    {
        if(!QDir("Downloaded/"+meetingID+"/Slides").exists())
        QDir().mkdir("Downloaded/"+meetingID+"/Slides");
        if(!QDir("Downloaded/"+meetingID+"/Slides/"+QString::number(i)).exists())
        {
          qDebug()<<"nist";
          QDir().mkdir("Downloaded/"+meetingID+"/Slides/"+QString::number(i));
        }
    QString slideUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/presentation/" +address_ha[i]+"/slide-";
        for(int j=0;j<2;j++)
        {
        request1.setUrl(slideUrl+QString::number(j+1)+".png");
        replay1 = manager1->get(request1);
        eventLoop.exec();
        QByteArray pic1 = replay1->readAll();
        QFile file15;
        file15.setFileName("Downloaded/"+meetingID+"/Slides/"+QString::number(i)+"/slide-"+QString::number(j+1));
        if(file15.open(QIODevice::WriteOnly))
          {
            file15.write(pic1);
            file15.close();
          }
        }
    }





}

void mainwindow::add_hexTounicode()
{
    hexTounicode.insert(QString("&#x620;"),QString("ي"));
    hexTounicode.insert(QString("&#x621;"),QString("ء"));
    hexTounicode.insert(QString("&#x622;"),QString("آ"));
    hexTounicode.insert(QString("&#x623;"),QString("أ"));
    hexTounicode.insert(QString("&#x624;"),QString("ؤ"));
    hexTounicode.insert(QString("&#x625;"),QString("إ"));
    hexTounicode.insert(QString("&#x626;"),QString("ئ"));
    hexTounicode.insert(QString("&#x627;"),QString("ا"));
    hexTounicode.insert(QString("&#x628;"),QString("ب"));
    hexTounicode.insert(QString("&#x629;"),QString("ة"));
    hexTounicode.insert(QString("&#x630;"),QString("ذ"));
    hexTounicode.insert(QString("&#x631;"),QString("ر"));
    hexTounicode.insert(QString("&#x632;"),QString("ز"));
    hexTounicode.insert(QString("&#x633;"),QString("س"));
    hexTounicode.insert(QString("&#x634;"),QString("ش"));
    hexTounicode.insert(QString("&#x635;"),QString("ص"));
    hexTounicode.insert(QString("&#x636;"),QString("ض"));
    hexTounicode.insert(QString("&#x637;"),QString("ط"));
    hexTounicode.insert(QString("&#x638;"),QString("ظ"));
    hexTounicode.insert(QString("&#x639;"),QString("ع"));
    hexTounicode.insert(QString("&#x640;"),QString("@"));
    hexTounicode.insert(QString("&#x641;"),QString("ف"));
    hexTounicode.insert(QString("&#x642;"),QString("ق"));
    hexTounicode.insert(QString("&#x643;"),QString("ک"));
    hexTounicode.insert(QString("&#x6A9;"),QString("ک"));
    hexTounicode.insert(QString("&#x644;"),QString("ل"));
    hexTounicode.insert(QString("&#x645;"),QString("م"));
    hexTounicode.insert(QString("&#x646;"),QString("ن"));
    hexTounicode.insert(QString("&#x647;"),QString("ه"));
    hexTounicode.insert(QString("&#x648;"),QString("و"));
    hexTounicode.insert(QString("&#x649;"),QString("ی"));
    hexTounicode.insert(QString("&#x661;"),QString("1"));
    hexTounicode.insert(QString("&#x662;"),QString("2"));
    hexTounicode.insert(QString("&#x663;"),QString("3"));
    hexTounicode.insert(QString("&#x664;"),QString("4"));
    hexTounicode.insert(QString("&#x665;"),QString("5"));
    hexTounicode.insert(QString("&#x666;"),QString("6"));
    hexTounicode.insert(QString("&#x667;"),QString("7"));
    hexTounicode.insert(QString("&#x668;"),QString("8"));
    hexTounicode.insert(QString("&#x669;"),QString("9"));
    hexTounicode.insert(QString("&#x686;"),QString("چ"));
    hexTounicode.insert(QString("&#x698;"),QString("ژ"));
    hexTounicode.insert(QString("&#x62D;"),QString("ح"));
    hexTounicode.insert(QString("&#x67E;"),QString("پ"));
    hexTounicode.insert(QString("&#x63A;"),QString("غ"));
    hexTounicode.insert(QString("&#x62E;"),QString("خ"));
    hexTounicode.insert(QString("&#x62C;"),QString("ج"));
    hexTounicode.insert(QString("&#x62D;"),QString("ح"));
    hexTounicode.insert(QString("&#x6AF;"),QString("گ"));
    hexTounicode.insert(QString("&#x62A;"),QString("ت"));
    hexTounicode.insert(QString("&#x62F;"),QString("د"));
    hexTounicode.insert(QString("&#x6CC;"),QString("ی"));
    hexTounicode.insert(QString("&#x2E;"),QString("."));
    hexTounicode.insert(QString("&#x21;"),QString("!"));
    hexTounicode.insert(QString("&#x23;"),QString("#"));
    hexTounicode.insert(QString("&#x24;"),QString("$"));
    hexTounicode.insert(QString("&#x25;"),QString("%"));
    hexTounicode.insert(QString("&#x5E;"),QString("^"));
    hexTounicode.insert(QString("&#x26;"),QString("&"));
    hexTounicode.insert(QString("&#x2A;"),QString("*"));
    hexTounicode.insert(QString("&#x28;"),QString("("));
    hexTounicode.insert(QString("&#x29;"),QString(")"));
    hexTounicode.insert(QString("&#x3C;"),QString("<"));
    hexTounicode.insert(QString("&#x3E;"),QString(">"));
    hexTounicode.insert(QString("&#x3F;"),QString("?"));
    hexTounicode.insert(QString("&#x2C;"),QString(","));
    hexTounicode.insert(QString("&#x7E;"),QString("~"));
    hexTounicode.insert(QString("&#x5F;"),QString("_"));
    hexTounicode.insert(QString("&#x2B;"),QString("+"));
    hexTounicode.insert(QString("&#x3D;"),QString("="));
    hexTounicode.insert(QString("&#x2D;"),QString("-"));
    hexTounicode.insert(QString("&#x61F;"),QString("؟"));
    hexTounicode.insert(QString("&#x3A;"),QString(":"));
    hexTounicode.insert(QString("&#x22;"),QString(""""));
    hexTounicode.insert(QString("&#x7B;"),QString("{"));
    hexTounicode.insert(QString("&#x7D;"),QString("}"));
    hexTounicode.insert(QString("&#x7C;"),QString("|"));
    hexTounicode.insert(QString("&#x2F;"),QString("/"));
    hexTounicode.insert(QString("&#x5C;"),QString("\\"));
    hexTounicode.insert(QString("&#x3B;"),QString(";"));
    hexTounicode.insert(QString("&#x27;"),QString("'"));
    hexTounicode.insert(QString("&#x30;"),QString("0"));
    hexTounicode.insert(QString("&#x61B;"),QString("؛"));
    hexTounicode.insert(QString("&#x60C;"),QString("،"));
    hexTounicode.insert(QString("&#x5D;"),QString("]"));
    hexTounicode.insert(QString("&#x5B;"),QString("["));

    hexTounicode.insert(QString("&#x51;"),QString("Q"));
    hexTounicode.insert(QString("&#x71;"),QString("q"));
    hexTounicode.insert(QString("&#x57;"),QString("W"));
    hexTounicode.insert(QString("&#x77;"),QString("w"));
    hexTounicode.insert(QString("&#x45;"),QString("E"));
    hexTounicode.insert(QString("&#x65;"),QString("e"));
    hexTounicode.insert(QString("&#x52;"),QString("R"));
    hexTounicode.insert(QString("&#x72;"),QString("r"));
    hexTounicode.insert(QString("&#x54;"),QString("T"));
    hexTounicode.insert(QString("&#x74;"),QString("t"));
    hexTounicode.insert(QString("&#x59;"),QString("Y"));
    hexTounicode.insert(QString("&#x79;"),QString("y"));
    hexTounicode.insert(QString("&#x55;"),QString("U"));
    hexTounicode.insert(QString("&#x75;"),QString("u"));
    hexTounicode.insert(QString("&#x49;"),QString("I"));
    hexTounicode.insert(QString("&#x69;"),QString("i"));
    hexTounicode.insert(QString("&#x4F;"),QString("O"));
    hexTounicode.insert(QString("&#x6F;"),QString("o"));
    hexTounicode.insert(QString("&#x50;"),QString("P"));
    hexTounicode.insert(QString("&#x70;"),QString("p"));
    hexTounicode.insert(QString("&#x41;"),QString("A"));
    hexTounicode.insert(QString("&#x61;"),QString("a"));
    hexTounicode.insert(QString("&#x53;"),QString("S"));
    hexTounicode.insert(QString("&#x73;"),QString("s"));
    hexTounicode.insert(QString("&#x44;"),QString("D"));
    hexTounicode.insert(QString("&#x64;"),QString("d"));
    hexTounicode.insert(QString("&#x46;"),QString("F"));
    hexTounicode.insert(QString("&#x66;"),QString("f"));
    hexTounicode.insert(QString("&#x47;"),QString("G"));
    hexTounicode.insert(QString("&#x67;"),QString("g"));
    hexTounicode.insert(QString("&#x48;"),QString("H"));
    hexTounicode.insert(QString("&#x68;"),QString("h"));
    hexTounicode.insert(QString("&#x4A;"),QString("J"));
    hexTounicode.insert(QString("&#x6A;"),QString("j"));
    hexTounicode.insert(QString("&#x4B;"),QString("K"));
    hexTounicode.insert(QString("&#x6B;"),QString("k"));
    hexTounicode.insert(QString("&#x4C;"),QString("L"));
    hexTounicode.insert(QString("&#x6C;"),QString("l"));
    hexTounicode.insert(QString("&#x5A;"),QString("Z"));
    hexTounicode.insert(QString("&#x7A;"),QString("z"));
    hexTounicode.insert(QString("&#58;"),QString("X"));
    hexTounicode.insert(QString("&#78;"),QString("x"));
    hexTounicode.insert(QString("&#x43;"),QString("C"));
    hexTounicode.insert(QString("&#x63;"),QString("c"));
    hexTounicode.insert(QString("&#x56;"),QString("V"));
    hexTounicode.insert(QString("&#x76;"),QString("v"));
    hexTounicode.insert(QString("&#x42;"),QString("B"));
    hexTounicode.insert(QString("&#x62;"),QString("b"));
    hexTounicode.insert(QString("&#x4E;"),QString("N"));
    hexTounicode.insert(QString("&#x6E;"),QString("n"));
    hexTounicode.insert(QString("&#x4D;"),QString("M"));
    hexTounicode.insert(QString("&#x6D;"),QString("m"));
}

QByteArray mainwindow::convertTounicode(QByteArray before)
{
    while(before.indexOf("&#x")!=-1)
    {
        QString temp=before.mid(before.indexOf("&#"),before.indexOf(";")-before.indexOf("&#")+1);
        before.replace(temp,hexTounicode.value(temp).toUtf8());
    }
    return before;
}




