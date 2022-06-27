QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    loading.cpp \
    main.cpp \
    mainwindow.cpp \
    myclient.cpp \
    mythread.cpp \
    siginup.cpp \
    signin.cpp \
    setting.cpp \
    search.cpp

HEADERS += \
    loading.h \
    mainwindow.h \
    myclient.h \
    mythread.h \
    siginup.h \
    signin.h \
    setting.h \
    search.h

FORMS += \
    loading.ui \
    mainwindow.ui \
    siginup.ui \
    signin.ui \
    setting.ui \
    search.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
