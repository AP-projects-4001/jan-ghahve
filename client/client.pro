QT       += core gui\
            network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Encryption/myencryption.cpp \
    Encryption/qaesencryption.cpp \
    adding_member.cpp \
    authenticationcode.cpp \
    channelpermissions.cpp \
    forgetpass.cpp \
    forwardbox.cpp \
    groupprofile.cpp \
    image_convertation.cpp \
    loading.cpp \
    main.cpp \
    mainwindow.cpp \
    msg_options.cpp \
    pvpermissions.cpp \
    setting.cpp \
    showingprof.cpp \
    siginup.cpp \
    signin.cpp \
    myclient.cpp \
    mythread.cpp \
    search.cpp \
    geraph.cpp \
    profile.cpp


HEADERS += \
    Encryption/aesni_enc_cbc.h \
    Encryption/aesni_enc_ecb.h \
    Encryption/aesni_key_exp.h \
    Encryption/myencryption.h \
    Encryption/qaesencryption.h \
    adding_member.h \
    authenticationcode.h \
    channelpermissions.h \
    forgetpass.h \
    forwardbox.h \
    groupprofile.h \
    image_convertation.h \
    loading.h \
    mainwindow.h \
    msg_options.h \
    pvpermissions.h \
    setting.h \
    showingprof.h \
    siginup.h \
    signin.h \
    myclient.h \
    mythread.h \
    geraph.h \
    search.h \
    profile.h

FORMS += \
    adding_member.ui \
    authenticationcode.ui \
    channelpermissions.ui \
    forgetpass.ui \
    forwardbox.ui \
    groupprofile.ui \
    loading.ui \
    mainwindow.ui \
    msg_options.ui \
    pvpermissions.ui \
    setting.ui \
    showingprof.ui \
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
    MyRes.qrc \
    MyRes.qrc \


