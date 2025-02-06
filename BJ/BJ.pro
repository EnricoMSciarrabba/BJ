QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cardblock.cpp \
    cardwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    manager.cpp \
    prob.cpp \
    waitingdialog.cpp

HEADERS += \
    cardblock.h \
    cardwidget.h \
    mainwindow.h \
    manager.h \
    prob.h \
    waitingdialog.h

FORMS += \
    cardblock.ui \
    mainwindow.ui \
    waitingdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc


RC_FILE = $$PWD/res/app_icon.rc

