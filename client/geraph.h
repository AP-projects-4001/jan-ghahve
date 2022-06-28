#ifndef GERAPH_H
#define GERAPH_H

#include <QDialog>

namespace Ui {
class geraph;
}

class geraph : public QDialog
{
    Q_OBJECT

public:
    explicit geraph(QWidget *parent = nullptr);
    ~geraph();

private:
    Ui::geraph *ui;
};

#endif // GERAPH_H
