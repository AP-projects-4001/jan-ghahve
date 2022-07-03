#include "myencryption.h"

#include <QDebug>
#include <iostream>
#include <QFile>
#include <QJsonArray>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

MyEncryption::MyEncryption()
{

}

QByteArray MyEncryption::myEncode(QByteArray plainText)
{
//    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB ,QAESEncryption::Padding::ZERO);
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    QByteArray encodedText = encryption.encode(plainText, KEY);
    return encodedText;
}

QByteArray MyEncryption::myDecode(QByteArray encodedText)
{
//    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB ,QAESEncryption::Padding::ZERO);
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB);
    QByteArray decodedText = encryption.decode(encodedText, KEY);
    qDebug()<<" &IN ENCRYPTION MYDECODE& : decode shod! ghabl az halghe hastim!";
    //remove padding at end of QByteArray
    if(decodedText.size()>0)
    {
        while(decodedText.back()==NULL)
        {
            decodedText.chop(1);
        }
        decodedText.chop(1);
    }
    return decodedText;
}
