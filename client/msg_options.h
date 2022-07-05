#ifndef MSG_OPTIONS_H
#define MSG_OPTIONS_H

#include <QDialog>

namespace Ui {
class Msg_Options;
}

class Msg_Options : public QDialog
{
    Q_OBJECT

public:
    explicit Msg_Options(QWidget *parent = nullptr);
    ~Msg_Options();

private:
    Ui::Msg_Options *ui;
};

#endif // MSG_OPTIONS_H
