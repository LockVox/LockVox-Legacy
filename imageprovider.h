#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include "cclient.h"
#include "clientlist.h"

#include <QAbstractListModel>
#include <QQuickImageProvider>
#include <QUuid>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider(ClientList * clientList,QImage img)
          : QQuickImageProvider(QQuickImageProvider::Image) {
        m_clientsList = clientList;
        default_img = img;

      }

    QImage requestImage(const QString &id, QSize *size,
                          const QSize &requestedSize) {
        int width = 100;
        int height = 50;
        Q_UNUSED(requestedSize)
        if (size)
          *size = QSize(width, height);

        //QUuid uuid(QByteArray::fromPercentEncoding(id.toLatin1()));
        qDebug() << "Looking for img of user : " << id << Qt::endl;
        foreach(CClient * c, m_clientsList->get_clients()){
            if(id == c->get_pseudo()){
                return c->get_profilePic();
            }
        }
        return default_img;
      }


private :
    ClientList * m_clientsList;
    QImage default_img;
};

#endif // IMAGEPROVIDER_H
