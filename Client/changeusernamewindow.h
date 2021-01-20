#ifndef CHANGEUSERNAMEWINDOW_H
#define CHANGEUSERNAMEWINDOW_H

#include <QWidget>

namespace Ui {
class ChangeUsernameWindow;
}

class ChangeUsernameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeUsernameWindow(QWidget *parent = nullptr);
    ~ChangeUsernameWindow();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ChangeUsernameWindow *ui;
};

#endif // CHANGEUSERNAMEWINDOW_H
