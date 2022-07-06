#ifndef IMAGECONVERTATION_H
#define IMAGECONVERTATION_H

#include <QJsonValue>
#include <QBuffer>
#include <QPixmap>

class ImageConvertation
{
public:
    ImageConvertation();
    QJsonValue jsonValFromPixmap(const QPixmap &p);
    QPixmap pixmapFrom(const QJsonValue &val);
};

#endif // IMAGECONVERTATION_H
