#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Hamed BBB-Player");

    start_program();
}

mainwindow::~mainwindow()
{
    delete ui;
}

void mainwindow::start_program()
{
    appSize.setWidth(QApplication::desktop()->availableGeometry().width());
    appSize.setHeight(QApplication::desktop()->availableGeometry().height() - (this->frameGeometry().height() - this->geometry().height())-20-ui->menubar->height());
    this->resize(appSize);


    add_hexTounicode();
    connect(ui->toolButton,&QToolButton::clicked,this,&mainwindow::setplayAndPause);


    mouse_lab=new QLabel(this);
    QPixmap pixmap1("cursor_red.png");
    mouse_lab->setPixmap(pixmap1.scaled(15,15,Qt::AspectRatioMode::KeepAspectRatio));
    mouse_lab->setFixedSize(15,15);
    mouse_lab->show();
    mouse_lab->setHidden(1);

    thr_player1 =new QThread(this);
    shortcut =new QShortcut(QKeySequence(Qt::Key::Key_Space), this, SLOT(setplayAndPause()));


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

       QDir().mkdir("Downloaded/");
       QDir().mkdir("Downloaded/"+meetingID);
     }


    //chat
    QString ChatUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/slides_new.xml";
    request1.setUrl(ChatUrl);
    replay1 = manager1->get(request1);
    connect(replay1,&QNetworkReply::downloadProgress,this,&mainwindow::manageSizeDownload);
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

    downloaded_complited+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();





    //cursor
    QString CursorUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/cursor.xml";
    request1.setUrl(CursorUrl);
    replay1 = manager1->get(request1);
    connect(replay1,&QNetworkReply::downloadProgress,this,&mainwindow::manageSizeDownload);
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

    downloaded_complited+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar->setValue((float)(downloaded_complited)/All_size);

    //metadata
    QString MetaData="https://blue.aut.ac.ir/presentation/"+meetingID+"/metadata.xml";
    request1.setUrl(MetaData);
    replay1 = manager1->get(request1);
    connect(replay1,&QNetworkReply::downloadProgress,this,&mainwindow::manageSizeDownload);
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

    downloaded_complited+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar->setValue((float)((downloaded_complited)*100/All_size));


    //desktopshare
    QString DeskshareUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/deskshare.xml";
    request1.setUrl(DeskshareUrl);
    replay1 = manager1->get(request1);
    connect(replay1,&QNetworkReply::downloadProgress,this,&mainwindow::manageSizeDownload);
    eventLoop.exec();
    QByteArray deskshare = replay1->readAll();
    QString namefileDetailDeskshare1="Downloaded/"+meetingID+"/"+"deskshare1.xml";
    detailDesshare_file.setFileName(namefileDetailDeskshare1);
    if(detailDesshare_file.open(QIODevice::ReadWrite))
    {
        detailDesshare_file.write(deskshare);
        detailDesshare_file.close();
    }

    downloaded_complited+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar->setValue((float)((downloaded_complited)*100/All_size));


//pak
    //deskshare_Asli just share screen
    QString DeskshareUrl1="https://blue.aut.ac.ir/presentation/"+meetingID+"/deskshare/deskshare.webm";
    request1.setUrl(DeskshareUrl1);
    replay1 = manager1->get(request1);
    connect(replay1,&QNetworkReply::downloadProgress,this,&mainwindow::manageSizeDownload);
    eventLoop.exec();
    deskshare1 = replay1->readAll();
    QString namefileDeskshare1="Downloaded/"+meetingID+"/"+"deskshare1.webm";
    deskshare1_file.setFileName(namefileDeskshare1);
    if(deskshare1_file.open(QIODevice::ReadWrite))
    {
       deskshare1_file.write(deskshare1);
       deskshare1_file.close();
    }


    downloaded_complited+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar->setValue((float)((downloaded_complited)*100/All_size));
//end pak

    //webcams_Asli and sound
//pak
    QString webcamsUrl1="https://blue.aut.ac.ir/presentation/"+meetingID+"/video/webcams.webm";
    request1.setUrl(webcamsUrl1);
    replay1 = manager1->get(request1);
    connect(replay1,&QNetworkReply::downloadProgress,this,&mainwindow::manageSizeDownload);
    eventLoop.exec();
    webcams = replay1->readAll();
    QString namefileWebcams="Downloaded/"+meetingID+"/"+"webcams.webm";
    webcams_file.setFileName(namefileWebcams);
    if(webcams_file.open(QIODevice::ReadWrite))
    {
       webcams_file.write(webcams);
       webcams_file.close();
    }



    downloaded_complited+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar->setValue((float)((downloaded_complited)*100/All_size));
//end pak

    //slides
    QString SourceSlideUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/presentation_text.json";
    request1.setUrl(SourceSlideUrl);
    replay1 = manager1->get(request1);
    connect(replay1,&QNetworkReply::downloadProgress,this,&mainwindow::manageSizeDownload);
    eventLoop.exec();
    sourceSilde = replay1->readAll();
    QString namefileSourceSlide="Downloaded/"+meetingID+"/"+"presentation_text.json";
    sourceSlide_file.setFileName(namefileSourceSlide);
    if(sourceSlide_file.open(QIODevice::ReadWrite))
    {
       sourceSlide_file.write(sourceSilde);
       sourceSlide_file.close();
    }
    downloaded_complited+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar->setValue((float)((downloaded_complited)*100/All_size));



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
               addad=i;
               break;
            }

        }
        QString tempi=str123.mid(re.indexIn(str123)-addad+1,54);
        address_ha.append(tempi);
        str123.replace(re.indexIn(str123),tempi.length(),"");
        matchs.append(tempi);
    }
        match_namefile.setFileName("Downloaded/"+meetingID+"/"+"matchs");
        if(match_namefile.open(QIODevice::WriteOnly))
        {
            for(int i=0;i<matchs.count();i++)
            {
                QString str1=matchs[i]+"\n";
                match_namefile.write(str1.toUtf8());

            }
        }

        match_namefile.close();
    //download slides
    for(int i=0;i<address_ha.count();i++)
    {
        if(!QDir("Downloaded/"+meetingID+"/Slides").exists())
        QDir().mkdir("Downloaded/"+meetingID+"/Slides");
        if(!QDir("Downloaded/"+meetingID+"/Slides/"+QString::number(i)).exists())
        {
          //make this dir
          QDir().mkdir("Downloaded/"+meetingID+"/Slides/"+QString::number(i));
        }
    QString slideUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/presentation/" +address_ha[i]+"/slide-";
        for(int j=0;j==0 or j<1000;j++)
        {
        request1.setUrl(slideUrl+QString::number(j+1)+".png");
        replay1 = manager1->get(request1);
        eventLoop.exec();
        QByteArray pic1 = replay1->readAll();
        if(pic1=="<html>\r\n<head><title>404 Not Found</title></head>\r\n<body>\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center>nginx</center>\r\n</body>\r\n</html>\r\n")
        {
            break;
        }
        QFile file15;
        file15.setFileName("Downloaded/"+meetingID+"/Slides/"+QString::number(i)+"/slide-"+QString::number(j+1));
        if(file15.open(QIODevice::WriteOnly))
          {
            file15.write(pic1);
            file15.close();
          }
        }
    }



    //moveing
    QString Moving="https://blue.aut.ac.ir/presentation/"+meetingID+"/shapes.svg";
    request1.setUrl(Moving);
    replay1 = manager1->get(request1);
    eventLoop.exec();
    moving = replay1->readAll();
    QString namefileMoving="Downloaded/"+meetingID+"/"+"shapes.xml";
    moving_file.setFileName(namefileMoving);
    if(moving_file.open(QIODevice::ReadWrite))
    {
       moving_file.write(moving);
       moving_file.close();
    }

    downloaded_complited+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
    progressBar->setValue((float)((downloaded_complited)*100/All_size));


    download_Dialog->close();

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
    hexTounicode.insert(QString("&#x62B;"),QString("ث"));
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



void mainwindow::player()
{
    //read All
    //read slides
    //address_downloaded="F:/c programer/winner 99.11/az-bbb/build-bbb-Desktop_Qt_5_15_2_MinGW_64_bit-Release/Downloaded/4469cae04424e7c447dd19a015934c7b800c64c9-1602415836554";
    QDir dir1(address_downloaded+"/Slides");
    dir1.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );
    for(int i=0;i<dir1.count();i++)
    {
        serial_Slide.append(struct_Slides1());
        QDir dir2(address_downloaded+"/Slides/"+QString::number(i));
        dir2.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot);
        for(int j=0;j<dir2.count();j++)
        {
            QFile fileTemp;
            fileTemp.setFileName(address_downloaded+"/Slides/"+QString::number(i)+"/slide-"+QString::number(j+1));
            if(fileTemp.open(QIODevice::ReadOnly))
            {
            serial_Slide[serial_Slide.count()-1].slide1.append(fileTemp.readAll());
            }
        }
    }



    QPixmap pixmap1;
    pixmap1.loadFromData(serial_Slide[0].slide1[0]);
    ui->label->setPixmap(pixmap1.scaled(ui->label->width(),ui->label->height(),Qt::AspectRatioMode::KeepAspectRatio));

    //read chats
    chat_file.setFileName(address_downloaded+"/chats.xml");
    if(chat_file.open(QIODevice::ReadOnly))
    {
        chats=chat_file.readAll();
    }
    for(int i=0;chats.indexOf("message=")!=-1;i++)
    {
    serial_Chat.append(struct_chat());
    serial_Chat[serial_Chat.count()-1].in=chats.mid(chats.indexOf("in=\"")+4,chats.indexOf("\" ")-chats.indexOf("in=\"")-4).toInt();
    serial_Chat[serial_Chat.count()-1].name=chats.mid(chats.indexOf("name=\"")+6,chats.indexOf("\" ",chats.indexOf("name=\""))-chats.indexOf("name=\"")-6);
    serial_Chat[serial_Chat.count()-1].massage=chats.mid(chats.indexOf("message=\"")+9,chats.indexOf("\" ",chats.indexOf("message=\""))-chats.indexOf("message=\"")-9);
    chats.remove(0,chats.indexOf("target=\"chat\"/>")+15);
    }


    //add match to slide stcuct
    match_namefile.setFileName(address_downloaded+"/matchs");
    if(match_namefile.open(QIODevice::ReadOnly))
    {
        for(int i=0;!match_namefile.atEnd();i++)
        {
        //serial_Slide[i].name=match_namefile.readLine();
        QString readedLine=match_namefile.readLine();
        convertNameSlideTonumber.insert(readedLine.replace(readedLine.indexOf('\n'),1,""),convertNameSlideTonumber.count());
        }
    }




    //read shape
    moving_file.setFileName(address_downloaded+"/shapes.xml");
    if(moving_file.open(QIODevice::ReadOnly))
    {
        moving=moving_file.readAll();
    }
    for(int i=0;moving.indexOf("in=\"")!=-1 ;i++)
    {
        serial_Moving.append(struct_moving());
        serial_Moving[i].in=moving.mid(moving.indexOf("in=\"")+4,moving.indexOf("\" ",moving.indexOf("in=\""))-moving.indexOf("in=\"")-4).toFloat();
        serial_Moving[i].out=moving.mid(moving.indexOf("out=\"")+5,moving.indexOf("\" ",moving.indexOf("out=\""))-moving.indexOf("out=\"")-5).toFloat();
        QString temp3=moving.mid(moving.indexOf("presentation/")+13,moving.indexOf("\" ",moving.indexOf("presentation/"))-moving.indexOf("presentation/")-13);
        if(temp3.indexOf("/slide-")!=-1)
        {
            serial_Moving[i].number_seriSlide=convertNameSlideTonumber.value(temp3.mid(0,temp3.indexOf("/")));
            serial_Moving[i].number_slide=temp3.mid(temp3.indexOf("/slide-")+7,moving.indexOf(".",moving.indexOf("/slide-"))-temp3.indexOf("/slide-")-7).toInt()-1;
            serial_Moving[i].number_slide=temp3.mid(temp3.indexOf("/slide-")+7,moving.indexOf(".",moving.indexOf("/slide-"))-temp3.indexOf("/slide-")-7).remove(".png").toInt()-1;
        }
        else
            if(temp3=="logo.png")
                serial_Moving[i].number_seriSlide=-2;
        else
            if(temp3=="deskshare.png")
                serial_Moving[i].number_seriSlide=-1;


        serial_Moving[i].wigth=moving.mid(moving.indexOf("width=\"")+7,moving.indexOf("\" ",moving.indexOf("width=\""))-moving.indexOf("width=\"")-7).toInt();
        serial_Moving[i].height=moving.mid(moving.indexOf("height=\"")+8,moving.indexOf("\" ",moving.indexOf("height=\""))-moving.indexOf("height=\"")-8).toInt();
        serial_Moving[i].x=moving.mid(moving.indexOf("x=\"")+3,moving.indexOf("\" ",moving.indexOf("x=\""))-moving.indexOf("x=\"")-3).toInt();
        serial_Moving[i].y=moving.mid(moving.indexOf("y=\"")+3,moving.indexOf("\" ",moving.indexOf("y=\""))-moving.indexOf("y=\"")-3).toInt();
        moving.remove(0,moving.indexOf("\"/>")+3);

    //*important* later should add shape example circle and path to this application
    }



    //
    //read curcer
    cursor_file.setFileName(address_downloaded+"/cursor.xml");
    if(cursor_file.open(QIODevice::ReadOnly))
    {
        while(!cursor_file.atEnd())
        cursor=cursor_file.readAll();
    }
    for(int i=0;cursor.indexOf("<event timestamp=")!=-1 ;i++)
    {
        serial_curcer.append(struct_curcer());
        serial_curcer[i].timestamp=cursor.mid(cursor.indexOf("timestamp=\"")+11,cursor.indexOf("\">",cursor.indexOf("timestamp=\""))-cursor.indexOf("timestamp=\"")-11).toFloat();
        serial_curcer[i].x=cursor.mid(cursor.indexOf("<cursor>")+8,cursor.indexOf(" ",cursor.indexOf("<cursor>"))-cursor.indexOf("<cursor>")-8).toFloat();
        cursor.remove(0,cursor.indexOf("<cursor>")+8);
        serial_curcer[i].y=cursor.mid(cursor.indexOf(" ")+1,cursor.indexOf("</cursor>")-cursor.indexOf(" ")-1).toFloat();
        cursor.remove(0,cursor.indexOf("</cursor>")+9);
    }



    //mouse
    timer_mouse = new QTimer(this);
       QList<struct_curcer> a=serial_curcer;
       connect(timer_mouse, &QTimer::timeout, this, [&,a]()
       {
           for(int i=0;i<a.count();i++)
           {
               //az shoro
               if(a[i].timestamp<=mediaPlayer1->position()*mediaPlayer1->duration()/1000)
               {

               if(i !=a.count()-1 and a[i+1].timestamp>=mediaPlayer1->position()*mediaPlayer1->duration()/1000)
               {
                   if(now_mouse!=i)
                   {
                       now_mouse=i;
                       checkNext(0,now_mouse);
                       break;
                   }
               }
               else
               {
                   continue;
               }

               }
               break;

               //az end

           /***
            if(i !=a.count()-1 and a[i+1].timestamp>=mediaPlayer1->position()*mediaPlayer1->duration()/1000)
            {
             if(i !=a.count()-2 and a[i+2].timestamp>=mediaPlayer1->position()*mediaPlayer1->duration()/1000)
              {
               continue;
              }
              else
              {
                if(now_mouse!=i)
                {
                now_mouse=i+1;
                checkNext(0,now_mouse);
                }
               break;
              }
            }
            ***/

        }
       });
       timer_mouse->start(timer_mouse_time);

       //slide
       timer_slide = new QTimer(this);
          QList<struct_moving> b=serial_Moving;
          connect(timer_slide, &QTimer::timeout, this, [&,b]()
          {
              for(int i=0;i<b.count();i++)
              {
              if(b[i].in<(mediaPlayer1->position()*mediaPlayer1->duration()/1000))
              {
              if(i !=b.count()-1 and b[i+1].in<=mediaPlayer1->position()*mediaPlayer1->duration()/1000)
                {

                  continue;
                }
              if(b[i].out>=mediaPlayer1->position()*mediaPlayer1->duration()/1000)
              {
                  if(b[i].number_seriSlide!=-1)
                  {

                  now_slide=i;

                  checkNext(1,now_slide);

                  }
                  else
                  {
                      now_slide=i;
                      checkNext(2,now_slide);
                  }
                  break;
              }
              }
            }
          });
          timer_slide->start(timer_slide_time);
     //chat

          timer_chat = new QTimer(this);
             QList<struct_chat> c=serial_Chat;
             connect(timer_chat, &QTimer::timeout, this, [&,c]()
             {
                for(int i=0;i<c.count();i++)
                {
                 if(c[i].in<(mediaPlayer1->position()*mediaPlayer1->duration()/1000))
                 {
                    if(i !=c.count()-1 and c[i+1].in>=(mediaPlayer1->position()*mediaPlayer1->duration()/1000))
                    {
                        if(now_chat!=i)
                        {
                        now_chat=i;
                        widgetChats->blockSignals(1);
                        widgetChats->clear();
                        widgetChats->blockSignals(0);
                             for(int j=0;j<=i;j++)
                             {
                             checkNext(3,j);
                             }
                        }
                    break;
                    }
                    else if(i==c.count()-1)
                    {
                       if(now_chat!=i)
                       {
                       now_chat=i;
                       checkNext(3,i);
                       }
                    }

                 }

                }
             });
          timer_chat->start(timer_chat_time);







    //play video


    videoWidget2 = new QVideoWidget(this);
    mediaPlayer2=new HamedMediaPlayer(this);

    if(QFile::exists(address_downloaded+"/deskshare1.webm"))
    {
    mediaPlayer2->setMedia(QUrl::fromLocalFile(address_downloaded+"/deskshare1.webm"));

    mediaPlayer2->setVideoOutput(videoWidget2);
    }

    mediaPlayer1=new HamedMediaPlayer(this);
    mediaPlayer1->moveToThread(thr_player1);
    mediaPlayer1->setMedia(QUrl::fromLocalFile(address_downloaded+"/webcams.webm"));
    QVideoWidget *videoWidget1 = new QVideoWidget(this);
    videoWidget1->show();


    mediaPlayer1->setVideoOutput(videoWidget1);
    mediaPlayer1->play();
    connect(mediaPlayer1,&HamedMediaPlayer::mediaStatusChanged,this,[&](int status){if(status==6) qApp->exit();});
    ui->toolButton->setText("Pause");



    ui->verticalLayout->addWidget(videoWidget1);

    widgetChats=new QListWidget(this);
    ui->verticalLayout->addWidget(widgetChats);


    //show
    ui->horizontalSlider->setRange(0,mediaPlayer1->duration()/1000);
    connect(ui->horizontalSlider,&QSlider::sliderMoved, this,[&](int addad123){mediaPlayer1->setPosition((float)addad123/(mediaPlayer1->duration()/1000));});
    connect(mediaPlayer1,&HamedMediaPlayer::positionChanged,this,&mainwindow::change_slider);

    widgetChats->setFixedWidth(appSize.width()*22/100);
    widgetChats->setFixedHeight(appSize.height()*47/100);
    videoWidget1->setFixedWidth(appSize.width()*22/100);


}

/*
void mainwindow::set_labelsize()
{
    ui->label->setFixedWidth(appSize.width()*78/100);
    ui->label->setFixedHeight(appSize.width()*95/100);
}
*/

void mainwindow::checkNext(int type,int Number)
{
    if(type==Mouse)
    {
        if(serial_curcer[Number].x!=-1 and  serial_curcer[Number].y!=-1)
        {
            //real mouse

            mouse_lab->setHidden(false);
            //30+13

            mouse_lab->setGeometry(((serial_curcer[Number].x*(ui->label->pixmap()->width())))+ui->label->x()-4,(serial_curcer[Number].y*(ui->label->pixmap()->height()))+ui->label->y()+ui->menubar->height()+((ui->label->height()-ui->label->pixmap()->height())/2)-5,mouse_lab->width(),mouse_lab->height());
        }
        else
        {
            mouse_lab->setHidden(1);
        }


    }


    else if(type==Slide)
    {

        this->showMaximized();
        if(mediaPlayer2->isPlaying())
        {
        mediaPlayer2->stop();
        played_player2=false;
        }
            videoWidget2->setHidden(1);
            ui->label->setHidden(0);
        if(serial_Moving.at(Number).number_seriSlide!=-2)
        {
            ui->label->setHidden(0);
            pic5.loadFromData(serial_Slide[serial_Moving.at(Number).number_seriSlide].slide1[serial_Moving.at(Number).number_slide]);
            ui->label->setPixmap(pic5.scaled(ui->label->width(),ui->label->height(),Qt::AspectRatioMode::KeepAspectRatio));
        }
    }

    else if(type==desktopShare)
    {
        videoWidget2->setHidden(0);
        videoWidget2->setGeometry(appSize.width()*23/100,ui->menubar->height()+10,ui->label->size().width(),ui->label->size().height());


        mediaPlayer2->play();
        played_player2=true;
        mediaPlayer2->setPosition((serial_Moving[Number].in*1000)/mediaPlayer2->duration());

    }

    else if(type==Chats)
    {
        QWidget* widget = new QWidget(this);
        QGridLayout* grid1 = new QGridLayout(this);
        QString nametime = serial_Chat[Number].name + "\n"/* + time*/;
        QLabel* lable4 = new QLabel(nametime, this);
        QTextEdit* text = new QTextEdit(this);
        text->setText(serial_Chat[Number].massage);
        text->setReadOnly(1);
        text->setStyleSheet("background-color: rgb(233, 255, 208)");
        grid1->addWidget(lable4, 0, 0);
        grid1->addWidget(text, 0, 1);


        widget->setLayout(grid1);

        QListWidgetItem* item = new QListWidgetItem;
        int hi = 47 + (serial_Chat[Number].massage.length() / 20) * 9;
        item->setSizeHint(QSize(150, hi));
        widgetChats->addItem(item);
        widgetChats->setItemWidget(item, widget);
        widgetChats->scrollToBottom();
    }
}

void mainwindow::download_gui()
{
    download_Dialog=new QDialog(this);

    dialog_layout=new QGridLayout(this);
    QLabel *lable_url=new QLabel(this);
    lable_url->setText("URL: ");
    QLineEdit *url_lineEdit=new QLineEdit(this);
    url_lineEdit->setMinimumWidth(400);
    QPushButton *btn_download=new QPushButton(this);
    btn_download->setText("Download");
    btn_download->setMaximumWidth(200);


    dialog_layout->addWidget(lable_url,0,0);
    dialog_layout->addWidget(url_lineEdit,0,1,1,6);
    dialog_layout->addWidget(btn_download,1,2);
    download_Dialog->setLayout(dialog_layout);
    download_Dialog->show();

    QGridLayout *dialog_layout1=dialog_layout;
    connect(btn_download,&QPushButton::clicked,this,[&,url_lineEdit,dialog_layout1,lable_url,btn_download](bool a){
        if(url_lineEdit->text()!="")
        {
            mainUrl=url_lineEdit->text();
            meetingID=mainUrl.mid(mainUrl.indexOf("/2.3/",0)+5,mainUrl.length()-mainUrl.indexOf("?",0)-11);
            QNetworkRequest request1;
                QNetworkAccessManager *manager1 = new QNetworkAccessManager(this);
                QNetworkReply *replay1;
                 QEventLoop eventLoop;
             QObject::connect(manager1, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);

            QString ChatUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/slides_new.xml";
            request1.setUrl(ChatUrl);
            replay1 = manager1->head(request1);
            eventLoop.exec();

            All_size=0;
            All_size+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();

            QString CursorUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/cursor.xml";
            request1.setUrl(CursorUrl);
            replay1 = manager1->head(request1);
            eventLoop.exec();
            All_size+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();
            QString MetaData="https://blue.aut.ac.ir/presentation/"+meetingID+"/metadata.xml";
            request1.setUrl(MetaData);
            replay1 = manager1->head(request1);
            eventLoop.exec();
            All_size+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();


            QString DeskshareUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/deskshare.xml";
            request1.setUrl(DeskshareUrl);
            replay1 = manager1->head(request1);
            eventLoop.exec();
            All_size+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();

            QString DeskshareUrl1="https://blue.aut.ac.ir/presentation/"+meetingID+"/deskshare/deskshare.webm";
            request1.setUrl(DeskshareUrl1);
            replay1 = manager1->head(request1);
            eventLoop.exec();
            All_size+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();

            QString webcamsUrl1="https://blue.aut.ac.ir/presentation/"+meetingID+"/video/webcams.webm";
            request1.setUrl(webcamsUrl1);
            replay1 = manager1->head(request1);
            eventLoop.exec();
            All_size+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();

            //slides
            QString SourceSlideUrl="https://blue.aut.ac.ir/presentation/"+meetingID+"/presentation_text.json";
            request1.setUrl(SourceSlideUrl);
            replay1 = manager1->head(request1);
            eventLoop.exec();
            All_size+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();


            QString Moving="https://blue.aut.ac.ir/presentation/"+meetingID+"/shapes.svg";
            request1.setUrl(Moving);
            replay1 = manager1->head(request1);
            eventLoop.exec();
            All_size+=replay1->header(QNetworkRequest::ContentLengthHeader).toULongLong();



        }

        progressBar= new QProgressBar(this);
        progressBar->setMinimum(0);
        progressBar->setMaximum(100);
        dialog_layout1->removeWidget(lable_url);
        dialog_layout1->removeWidget(url_lineEdit);
        dialog_layout1->removeWidget(btn_download);
        lable_url->setHidden(1);
        url_lineEdit->setHidden(1);
        btn_download->setHidden(1);


        dialog_layout1->addWidget(progressBar,0,0);
        downloader();

    });

}

void mainwindow::manageSizeDownload(qint64 bytesReceived, qint64 bytesTotal)
{
    int value=(float)((bytesReceived+downloaded_complited)*100/(All_size));
    progressBar->setValue(value);
}

void mainwindow::change_slider(float pos)
{
    ui->horizontalSlider->setValue(pos*mediaPlayer1->duration()/1000);
    QString now_time=QString::number((int)(pos*mediaPlayer1->duration())/(1000*60))+":"+QString::number((int)(pos*mediaPlayer1->duration())%(1000*60)/1000);
    QString duratin=QString::number((int)(mediaPlayer1->duration())/(1000*60))+":"+QString::number((int)(mediaPlayer1->duration())%(1000*60)/1000);
    ui->label_2->setText(now_time+"/"+duratin);
}

void mainwindow::setplayAndPause()
{
    if(mediaPlayer1->isPlaying())
    {
        //should stop
        if(mediaPlayer2->isPlaying())
        played_player2=true;
        else
        played_player2=false;


        mediaPlayer1->pause();
        timer_mouse->stop();
        timer_slide->stop();
        timer_chat->stop();

        ui->toolButton->setText("Play  ");

    }
    else if(!mediaPlayer1->isPlaying())
    {
        //should play

        mediaPlayer1->play();
        if(played_player2==true)
            mediaPlayer2->play();

        timer_mouse->start(timer_mouse_time);
        timer_slide->start(timer_slide_time);
        timer_chat->start(timer_chat_time);

        ui->toolButton->setText("Pause");
    }

}


void mainwindow::on_actionDownload_triggered()
{
    download_gui();
}


void mainwindow::on_actionOpen_File_triggered()
{
   address_downloaded = QFileDialog::getExistingDirectory(this, tr("Open Directory"),QApplication::applicationDirPath()+"/Downloaded/",
                                                            QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);

    player();
}



void mainwindow::on_actionAbout_triggered()
{
    QMessageBox message;
    message.setText("Hamed BBB-player is developed by hamed shakib\nPowered from Qt 5.15.2 and VLC 3.0.8");
    message.setButtonText(1,"Ok");
    message.exec();
}
