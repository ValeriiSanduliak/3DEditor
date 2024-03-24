#include "mainwindow.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSplitter>
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

    // QVBoxLayout *layoutv = new QVBoxLayout();
    // QHBoxLayout *layout = new QHBoxLayout();

    // QSplitter *splitter = new QSplitter(Qt::Vertical);

    // for (QCheckBox *checkBox : checkBoxes) {
    //     layoutv->addWidget(checkBox);
    //     layoutv->setStretchFactor(checkBox, 0);
    // }
    // layoutv->setContentsMargins(0, 0, 0, 500);
    // layoutv->addWidget(splitter);
    // layout->addLayout(layoutv);
    // ui->widget->setContentsMargins(0, 0, 0, 0);
    // layout->addWidget(ui->widget);

    // layout->setStretchFactor(ui->widget, 1);

    // ui->centralwidget->setLayout(layout);

    ui->horizontalLayout_6->setStretchFactor(ui->widget, 1);
    ui->verticalLayout_7->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    ui->centralwidget->setLayout(ui->horizontalLayout_6);

    ui->menuBar->setStyleSheet("background-color: rgb(156, 156, 156);");
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

void MainWindow::on_actionLoad_model_obj_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    QString("C://"),
                                                    tr("Object Files (*.obj)"));

    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString extension = fileInfo.suffix();

        if (extension.toLower() != "obj") {
            QMessageBox::critical(this, tr("Error"), tr("Selected file is not an OBJ file."));
            return;
        }

        ui->widget->loadObjectFromFile(fileName);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        QString("C://"),
        tr("Png Files (*.png);;Jpeg Files (*.jpeg);;Bmp Files (*.bmp)"));
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        QString extension = fileInfo.suffix();

        if (extension.toLower() != "png" && extension.toLower() != "jpeg"
            && extension.toLower() != "bmp") {
            QMessageBox::critical(this, tr("Error"), tr("Invalid format file."));
            return;
        }
        ui->widget->setTexture(fileName);
    }
}
