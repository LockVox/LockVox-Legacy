#ifndef USERSTATUSWINDOW_H
#define USERSTATUSWINDOW_H

#include <QWidget>

namespace Ui {
class userStatusWindow;
}

class userStatusWindow : public QWidget
{
    Q_OBJECT

public:
    explicit userStatusWindow(QWidget *parent = nullptr);
    ~userStatusWindow();

private:
    Ui::userStatusWindow *ui;
};

#endif // USERSTATUSWINDOW_H
