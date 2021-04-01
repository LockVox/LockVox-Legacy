#include "imageprovider.h"



ClientList *ImageProvider::clientsList() const
{
    return m_clientsList;
}

void ImageProvider::setClientsList(ClientList *clientsList)
{
    m_clientsList = clientsList;
}
