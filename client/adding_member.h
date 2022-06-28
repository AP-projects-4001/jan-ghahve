#ifndef ADDING_MEMBER_H
#define ADDING_MEMBER_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class adding_member;
}

class adding_member : public QDialog
{
    Q_OBJECT

public:
    explicit adding_member(QWidget *parent = nullptr);
    ~adding_member();

private:
    Ui::adding_member *ui;
};

#endif // ADDING_MEMBER_H
