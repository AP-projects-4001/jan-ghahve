QT       += core gui\
            network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    loading.cpp \
    main.cpp \
    mainwindow.cpp \
    setting.cpp \
    siginup.cpp \
    signin.cpp \
    myclient.cpp \
    mythread.cpp \
    search.cpp \
    geraph.cpp \
    profile.cpp


HEADERS += \
    loading.h \
    mainwindow.h \
    setting.h \
    siginup.h \
    signin.h \
    myclient.h \
    mythread.h \
    geraph.h \
    search.h \
    profile.h

FORMS += \
    loading.ui \
    mainwindow.ui \
    setting.ui \
    siginup.ui \
    signin.ui \
    profile.ui \
    search.ui \
    geraph.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    MyRes.qrc
