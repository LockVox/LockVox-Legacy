#ifndef CPASSWORD_H
#define CPASSWORD_H

#include <QString>


class cpassword
{
    public:

        cpassword();
        ~cpassword();

        void setuserId(QString userId); //Database
        QString getuserId();

        void setuserPassword(QString password); // received user password

        void setuserHasedPassword(QString hashedPassword); // hash of received  user password
        QString getuserHashedPassword();

        void setuserHash(QString hash); //Database stored hashed password
        QString getuserHash();

    private:
        QString userId;
        QString password;
        QString hash;
        QString hashedPassword;
        bool authentified;
};

#endif // CPASSWORD_H
