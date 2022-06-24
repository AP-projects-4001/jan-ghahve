#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>

namespace Ui {
class signin;
}

class signin : public QDialog
{
    Q_OBJECT

public:
    explicit signin(QWidget *parent = nullptr);
    ~signin();

private:
    Ui::signin *ui;
};

#endif // SIGNIN_H
