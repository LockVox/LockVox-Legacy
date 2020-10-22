#ifndef LOCKVOXMAINWINDOW_H
#define LOCKVOXMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class LockVoxMainWindow;
}

class LockVoxMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LockVoxMainWindow(QWidget *parent = nullptr);
    ~LockVoxMainWindow();

private:
    Ui::LockVoxMainWindow *ui;
};

#endif // LOCKVOXMAINWINDOW_H
