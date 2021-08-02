#include "crole.h"

crole::crole()
{
    m_father = NULL;
    m_write = false;
    m_speak = false;

}

crole::crole(crole* p_father)
{
    m_father = p_father;
    m_write = p_father->m_write;
    m_speak = p_father->m_speak;
    m_kick = p_father->m_kick;
    m_delete = p_father->m_delete;
    m_see = p_father->m_see;
    m_mute = p_father->m_mute;
}

/*********************************************************************
****************************FATHER+CHILD******************************
**********************************************************************/

void crole::AddFather(crole *p_father)
{
    if(p_father!=NULL)
        qDebug() << "Father replaced!\n";
    else
        qDebug() << "Father added!\n";
    m_father = p_father;
    return;
}

void crole::AddChild(crole *p_child)
{
    m_childrens.append(p_child);
}

void crole::DelChild(QString p_name)
{
    int i=0;
    foreach(crole cur, m_childrens)
    {
        if(cur.m_name == p_name)
        {
            m_childrens.removeAt(i);
        }
        i++;
    }
}

void crole::DelChild(int p_id)
{
    int i=0;
    foreach(crole cur, m_childrens)
    {
        if(cur.m_id == p_id)
        {
            m_childrens.removeAt(i);
        }
        i++;
    }
}

/*********************************************************************
****************************GETTER+SETTER******************************
**********************************************************************/

void crole::SetWrite(bool v)
{
    m_write =v;
}

void crole::SetSpeak(bool v)
{
    m_speak = v;
}

void crole::SetKick(bool v)
{
    m_kick = v;
}

void crole::SetDelete(bool v)
{
    m_delete = v;
}

void crole::SetSee(bool v)
{
    m_see = v;
}

void crole::SetMute(bool v)
{
    m_mute = v;
}

/******************************************/

bool crole::GetWrite()
{
    return m_write;
}

bool crole::GetSpeak()
{
    return m_speak;
}

bool crole::GetKick()
{
    return m_kick;
}

bool crole::GetDelete()
{
    return m_delete;
}

bool crole::GetSee()
{
    return m_see;
}

bool crole::GetMute()
{
    return m_mute;
}
