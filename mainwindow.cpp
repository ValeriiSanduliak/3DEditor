#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setFocusPolicy(Qt::StrongFocus);
    //QList<QCheckBox *>
    checkBoxes = findChildren<QCheckBox *>();

    // Пройтися по знайденим checkbox і встановити посилання на кожен з них у WidgetGL
    for (QCheckBox *checkBox : checkBoxes) {
        ui->widget->setCheckBox(checkBox);
        connect(checkBox, &QCheckBox::clicked, this, &MainWindow::checkBoxClicked);
    }
}
void MainWindow::checkBoxClicked()
{
    QCheckBox *clickedCheckBox = qobject_cast<QCheckBox *>(sender());

    if (clickedCheckBox->isChecked()) {
        // Якщо вибраний один прапорець, зняти вибір з усіх інших
        for (QCheckBox *checkBox : checkBoxes) {
            if (checkBox != clickedCheckBox) {
                checkBox->setChecked(false);
            }
        }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
