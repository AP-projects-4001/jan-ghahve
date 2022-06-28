#ifndef SHOWINGPROF_H
#define SHOWINGPROF_H

#include <QWidget>

namespace Ui {
class showingProf;
}

class showingProf : public QWidget
{
    Q_OBJECT

public:
    explicit showingProf(QWidget *parent = nullptr);
    ~showingProf();

private:
    Ui::showingProf *ui;
};

#endif // SHOWINGPROF_H
