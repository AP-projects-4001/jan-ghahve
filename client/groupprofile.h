#ifndef GROUPPROFILE_H
#define GROUPPROFILE_H

#include <QDialog>

namespace Ui {
class GroupProfile;
}

class GroupProfile : public QDialog
{
    Q_OBJECT

public:
    explicit GroupProfile(QString id,QWidget *parent = nullptr);
    ~GroupProfile();

private slots:
    void on_pbn_permission_clicked();

private:
    Ui::GroupProfile *ui;

    QString id;
};

#endif // GROUPPROFILE_H
