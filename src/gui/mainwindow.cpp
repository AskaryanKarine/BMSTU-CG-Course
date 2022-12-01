#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QColorDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QTimer>

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

    auto bc = picture.get_backColor();
    auto sc = picture.get_mainSphereColor();

    set_color(sc, ui->lbl_sphereColor);
    set_color(tmp, ui->lbl_figColor);
    set_color(bc, ui->lbl_backgroundColor);
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

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    const int image_width = ui->graphicsView->width();
    const int image_height = ui->graphicsView->height();
    picture.set_height(image_height);
    picture.set_width(image_width);
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
    QColor tmpC = picture.get_mainSphereColor();
    color_dialog(tmpC);
    set_color(tmpC, ui->lbl_sphereColor);
    picture.set_mainSphereColor(tmpC);
}

void MainWindow::showEvent(QShowEvent* ev) // когда окно полностью сконструировано
{
    QMainWindow::showEvent(ev);
    QTimer::singleShot(50, this, SLOT(windowShown()));
}

void MainWindow::windowShown() // начальный вывод на экран
{
    const int image_width = ui->graphicsView->width();
    const int image_height = ui->graphicsView->height();
    picture.set_height(image_height);
    picture.set_width(image_width);
}

void MainWindow::on_pB_figColor_clicked()
{
}

void MainWindow::on_pB_backgroundColor_clicked()
{
    QColor tmpC = picture.get_backColor();
    color_dialog(tmpC);
    set_color(tmpC, ui->lbl_backgroundColor);
    picture.set_backColor(tmpC);
}

void MainWindow::color_dialog(QColor& color)
{
    QColorDialog dialog;
    dialog.setCurrentColor(color);
    dialog.show();
    dialog.exec();
    QColor tmp = dialog.selectedColor();
    if (!tmp.isValid())
        show_error("Выберите цвет");
    else
        color = tmp;
}

void MainWindow::on_pB_draw_clicked()
{
    // Настройка
    double n = ui->dSB_sphereReflection->value();
    picture.set_refIndex(n);
    double tr = ui->dSB_sphereTransparency->value();
    picture.set_transparient(tr);

    // Рисование
    img = picture.drawingFgure();
    QPixmap pxm = QPixmap::fromImage(*img);
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(pxm);
}
