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
    explicit GroupProfile(QString id,QString visitor ,QWidget *parent = nullptr);
    ~GroupProfile();

private slots:
    void on_pbn_permission_clicked();

    void on_pbn_change_image_clicked();


    void on_pbn_save_clicked();

    void on_pbn_cancel_clicked();

private:
    Ui::GroupProfile *ui;
    QString id;
    QPixmap profile_pix;
    QPixmap new_profile_pix;
};

#endif // GROUPPROFILE_H
