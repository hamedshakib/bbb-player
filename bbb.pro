QT += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    hamedmediaplayer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    hamedmediaplayer.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=$$PWD/Files/include

LIBS += -L$$PWD/Files
LIBS += -laxvlc -llibvlc -llibvlccore -lnpvlc
LIBS += -llibcrypto-1_1-x64 -llibssl-1_1-x64 -lvcruntime140_1

RC_FILE = bbb-player.rc
