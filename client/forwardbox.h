#ifndef FORWARDBOX_H
#define FORWARDBOX_H

#include <QDialog>

namespace Ui {
class ForwardBox;
}

class ForwardBox : public QDialog
{
    Q_OBJECT

public:
    explicit ForwardBox(QWidget *parent = nullptr);
    ~ForwardBox();

private:
    Ui::ForwardBox *ui;
};

#endif // FORWARDBOX_H
