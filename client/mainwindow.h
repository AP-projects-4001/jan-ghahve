#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void add_safebar();

private slots:
    void on_test_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);


private:
    Ui::MainWindow *ui;
    void pain();
};
#endif // MAINWINDOW_H
