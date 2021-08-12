#include "src/includes/LVObject.h"

CObject::CObject()
{
    m_obj = UNKNOWN;
}

CObject::ObjectType CObject::getObjType() const
{
    return m_obj;
}



void CObject::setObjType(const ObjectType &obj)
{
    m_obj = obj;
}
