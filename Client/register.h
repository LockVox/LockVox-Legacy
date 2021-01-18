#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class register; }
QT_END_NAMESPACE


namespace Ui {
class register;
}

class register : public QMainWindow
{
    Q_OBJECT

public:
    explicit register(QWidget *parent = nullptr);
    ~register();

private:
    Ui::register *ui;
};

#endif // REGISTER_H
