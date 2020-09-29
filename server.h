#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>

class server : public QMainWindow
{
    Q_OBJECT

public:
    server(QWidget *parent = nullptr);
    ~server();
};
#endif // SERVER_H
