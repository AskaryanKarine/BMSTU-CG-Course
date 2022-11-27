#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QColorDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QTimer>

#include <iostream>

#include "lightsource.h"
#include "sphere.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QAction* HelpAction = ui->menubar->addAction("Помощь");
    //        connect(HelpAction, SIGNAL(triggered()), this, SLOT(author_info_show()));
    QAction* ExitAction = ui->menubar->addAction(("Выход"));
    connect(ExitAction, SIGNAL(triggered()), this, SLOT(app_exit()));

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->clear();
}

MainWindow::~MainWindow()
{
    delete ui->graphicsView->scene();
    delete ui;
}

void MainWindow::show_error(QString str)
{
    QMessageBox::critical(NULL, "Ошибка", str);
}

void MainWindow::app_exit()
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

void MainWindow::set_color(QColor color, QLabel* lab)
{
    QImage img = QImage(lab->geometry().width(), lab->geometry().height(), QImage::Format_RGB32);
    QPainter p(&img);
    p.setBrush(QBrush(color));
    p.setPen(Qt::black);
    QRect rect = QRect(0, 0, lab->geometry().width(), lab->geometry().height());
    p.drawRect(rect);

    QPixmap pixmap = QPixmap::fromImage(img);
    lab->clear();
    lab->setPixmap(pixmap);
}

void MainWindow::on_pB_sphereColor_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(tmp);
    dialog.show();
    dialog.exec();
    QColor color = dialog.selectedColor();
    if (!color.isValid())
        show_error("Выберите цвет");
    else
        tmp = color;
    set_color(tmp, ui->lbl_sphereColor);
}

void MainWindow::showEvent(QShowEvent* ev) // когда окно полностью сконструировано
{
    QMainWindow::showEvent(ev);
    QTimer::singleShot(50, this, SLOT(windowShown()));
}

void MainWindow::windowShown() // начальный вывод на экран
{
    //    std::cout << ui->groupBox_4->width() << std::endl;
    const int image_width = ui->graphicsView->width();
    const int image_height = ui->graphicsView->height();
    //    pic.set_height(image_height);
    //    pic.set_width(image_width);
}
