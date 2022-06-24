#ifndef SIGINUP_H
#define SIGINUP_H

#include <QWidget>

namespace Ui {
class siginup;
}

class siginup : public QWidget
{
    Q_OBJECT

public:
    explicit siginup(QWidget *parent = nullptr);
    ~siginup();

private:
    Ui::siginup *ui;
};

#endif // SIGINUP_H
