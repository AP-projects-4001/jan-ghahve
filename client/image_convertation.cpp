#include "image_convertation.h"


ImageConvertation::ImageConvertation()
{

}

//convert QPixmap to QJsonValue (for sending in socket and write it to file in database)
QJsonValue ImageConvertation::jsonValFromPixmap(const QPixmap &p)
{
  QBuffer buffer;
  buffer.open(QIODevice::WriteOnly);
  p.save(&buffer, "PNG");
  auto const encoded = buffer.data().toBase64();
  return {QLatin1String(encoded)};
}

//convert QJsonValue to QPixmap  (for showing on label in user-window)
QPixmap ImageConvertation::pixmapFrom(const QJsonValue &val)
{
  auto const encoded = val.toString().toLatin1();
  QPixmap p;
  p.loadFromData(QByteArray::fromBase64(encoded), "PNG");
  return p;
}
