#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCheckBox>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void checkBoxClicked();

private slots:
    void on_actionLoad_model_obj_triggered();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QCheckBox *> checkBoxes;
};
#endif // MAINWINDOW_H
