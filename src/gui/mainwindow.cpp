#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QColorDialog>
#include <QGraphicsScene>
#include <QMessageBox>

#include "sphere.h"
#include "lightsource.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QAction* SettingAction = ui->menubar->addAction("Настройки сцены");
    //        connect(SettingAction, SIGNAL(triggered()), this, SLOT(app_info_show()));
    QAction* HelpAction = ui->menubar->addAction("Помощь");
    //        connect(HelpAction, SIGNAL(triggered()), this, SLOT(author_info_show()));
    QAction* ExitAction = ui->menubar->addAction(("Выход"));
    connect(ExitAction, SIGNAL(triggered()), this, SLOT(exit_show()));

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->clear();

    Sphere a();
    LightSource b();
}

MainWindow::~MainWindow()
{
    delete ui->graphicsView->scene();
    delete ui;
}

void MainWindow::error_message(QString str)
{
    QMessageBox::critical(NULL, "Ошибка", str);
}

void MainWindow::exit_show()
{
    QMessageBox msg_quit;
    msg_quit.setText("Работа с программой будет завершена");
    msg_quit.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg_quit.setIcon(QMessageBox::Warning);
    msg_quit.setDefaultButton(QMessageBox::Ok);
    msg_quit.setWindowTitle("Предупреждение!");
    int rc = msg_quit.exec();

    if (rc == QMessageBox::Ok)
        qApp->quit();
}

void MainWindow::show_color(QColor color, QLabel* lab)
{
    QImage im = QImage(lab->geometry().width(), lab->geometry().height(), QImage::Format_RGB32);
    QPainter p(&im);
    p.setBrush(QBrush(color));
    p.setPen(Qt::black);
    QRect rect = QRect(0, 0, lab->geometry().width(), lab->geometry().height());
    p.drawRect(rect);

    QPixmap pixmap = QPixmap::fromImage(im);
    lab->clear();
    lab->setPixmap(pixmap);
}

void MainWindow::on_pushButton_sph_color_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(tmp);
    dialog.show();
    dialog.exec();
    QColor color = dialog.selectedColor();
    if (!color.isValid())
        error_message("Выберите цвет");
    else
        tmp = color;
    show_color(tmp, ui->label_sph_color);
}
