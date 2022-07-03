#ifndef MYENCRYPTION_H
#define MYENCRYPTION_H

#include <QObject>
#include "qaesencryption.h"

class MyEncryption
{
public:
    MyEncryption();
    QByteArray myEncode(QByteArray plainText);
    QByteArray myDecode(QByteArray encodedText);

private:
    QByteArray KEY ="729308A8E815F6A46EB3A8AE6D5463CA7B64A0E2E11BC26A68106FC7697E727E37011";
};

#endif // MYENCRYPTION_H
