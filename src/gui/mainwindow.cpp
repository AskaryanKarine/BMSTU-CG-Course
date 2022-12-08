#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <QColorDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <iostream>

#include <QTextStream>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QAction* ExitAction = ui->menubar->addAction(("Выход"));
    connect(ExitAction, SIGNAL(triggered()), this, SLOT(app_exit()));

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->scene()->clear();

    auto bc = picture.get_backColor();
    auto sc = picture.get_mainSphereColor();
    figColor = QColor(255, 69, 0);

    set_color(sc, ui->lbl_sphereColor);
    set_color(figColor, ui->lbl_figColor);
    set_color(bc, ui->lbl_backgroundColor);
    camera_step = 100;
    auto cp = picture.get_cam_pos();
    QString p;
    p = p.asprintf("Камера: (%.0f, %.0f, %.0f)", cp.x(), cp.y(), cp.z());
    ui->label_pos->setText(p);

    int cLight = picture.get_count_light();
    for (int i = 0; i < cLight; i++) {
        p = "Точечный источник света " + QString::number(i + 1);
        ui->cB_lightAll->addItem(p);
    }
    ui->cB_lightAll->setCurrentIndex(ui->cB_lightAll->count() - 1);
    //    QString settingStyle = "#centralwidget { background-color: rgb(49, 32, 56); }";
    //    this->setStyleSheet(settingStyle);
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
    color_dialog(figColor);
    set_color(figColor, ui->lbl_figColor);
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
    picture.set_transparient(1 - tr);

    // Рисование
    img = picture.drawingFigure(16);
    QPixmap pxm = QPixmap::fromImage(*img);
    ui->graphicsView->scene()->clear();
    ui->graphicsView->scene()->addPixmap(pxm);
}

void MainWindow::on_pB_cameraLeft_clicked()
{
    QVector3D offset(camera_step, 0, 0);
    picture.move_camera(offset);
    auto cp = picture.get_cam_pos();
    QString p;
    p = p.asprintf("Камера: (%.0f, %.0f, %.0f)", cp.x(), cp.y(), cp.z());
    ui->label_pos->setText(p);
}

void MainWindow::on_pB_cameraUp_clicked()
{
    QVector3D offset(0, camera_step, 0);
    picture.move_camera(offset);
    auto cp = picture.get_cam_pos();
    QString p;
    p = p.asprintf("Камера: (%.0f, %.0f, %.0f)", cp.x(), cp.y(), cp.z());
    ui->label_pos->setText(p);
}

void MainWindow::on_pB_cameraDown_clicked()
{
    QVector3D offset(0, -camera_step, 0);
    picture.move_camera(offset);
    auto cp = picture.get_cam_pos();
    QString p;
    p = p.asprintf("Камера: (%.0f, %.0f, %.0f)", cp.x(), cp.y(), cp.z());
    ui->label_pos->setText(p);
}

void MainWindow::on_pB_cameraRight_clicked()
{
    QVector3D offset(-camera_step, 0, 0);
    picture.move_camera(offset);
    auto cp = picture.get_cam_pos();
    QString p;
    p = p.asprintf("Камера: (%.0f, %.0f, %.0f)", cp.x(), cp.y(), cp.z());
    ui->label_pos->setText(p);
}

void MainWindow::on_pB_cameraZoomPlus_clicked()
{
    QVector3D offset(0, 0, camera_step);
    picture.move_camera(offset);
    auto cp = picture.get_cam_pos();
    QString p;
    p = p.asprintf("Камера: (%.0f, %.0f, %.0f)", cp.x(), cp.y(), cp.z());
    ui->label_pos->setText(p);
}

void MainWindow::on_pB_cameraZoomMinus_clicked()
{
    QVector3D offset(0, 0, -camera_step);
    picture.move_camera(offset);
    auto cp = picture.get_cam_pos();
    QString p;
    p = p.asprintf("Камера: (%.0f, %.0f, %.0f)", cp.x(), cp.y(), cp.z());
    ui->label_pos->setText(p);
}

void MainWindow::on_pB_figAdd_clicked()
{
    int type_id = ui->cB_figType->currentIndex();
    picture.added_model(type_id, figColor);

    auto a = ui->cB_figType->currentText();
    QString p;
    int curId = ui->cB_figAll->count() + 1;
    p = a + " " + QString::number(curId);
    ui->cB_figAll->addItem(p);
    ui->cB_figAll->setCurrentIndex(ui->cB_figAll->count() - 1);
}

void MainWindow::on_pB_lightAdd_clicked()
{
    picture.added_light();
    int curId = ui->cB_lightAll->count() + 1;
    QString p = "Точечный источник света " + QString::number(curId);
    ui->cB_lightAll->addItem(p);
    ui->cB_lightAll->setCurrentIndex(ui->cB_lightAll->count() - 1);
}

void MainWindow::on_pB_lightDel_clicked()
{
    int curId = ui->cB_lightAll->currentIndex();
    int lcnt = picture.get_count_light();
    if (lcnt == 0) {
        show_error("Удалять нечего");
        return;
    }
    picture.remove_light(curId);
    ui->cB_lightAll->removeItem(curId);
}

void MainWindow::on_pB_figDel_clicked()
{
    int curId = ui->cB_figAll->currentIndex() + 2;
    int mcnt = picture.get_count_models();
    if (mcnt == 2) {
        show_error("Удалять нечего");
        return;
    }
    picture.remove_model(curId);
    ui->cB_figAll->removeItem(curId - 2);
}

void MainWindow::on_pB_monig_all_clicked()
{
    QVector3D move_light(ui->sB_lightMoveX->value(), ui->sB_lightMoveY->value(), ui->sB_lightMoveZ->value());
    int light_id = ui->cB_lightAll->currentIndex();
    if (picture.get_count_light() > 0)
        picture.trasform_light(light_id, move_light);

    QVector3D move_fig(ui->sB_figMoveX->value(), ui->sB_figMoveY->value(), ui->sB_figMoveZ->value());
    QVector3D scale_fig(ui->sB_figScaleX->value(), ui->sB_figScaleY->value(), ui->sB_figScaleZ->value());
    QVector3D rotate_fig(ui->sB_figRotateX->value(), ui->sB_figRotateY->value(), ui->sB_figRotateZ->value());
    int fig_id = ui->cB_figAll->currentIndex() + 2;
    if (picture.get_count_models() - 2 > 0) {
        picture.transform_model(fig_id, move_fig, scale_fig, rotate_fig);
        picture.change_fig_color(fig_id, figColor);
    }
}

void MainWindow::on_cB_figAll_currentIndexChanged(int index)
{
    QString a = ui->cB_figAll->currentText();
    auto n = a.split(" ");
    bool flag = true;
    if (!QString::compare(n[0], QString("Шар"), Qt::CaseInsensitive)) {
        flag = false;
    }
    ui->sB_figRotateY->setEnabled(flag);
    ui->sB_figRotateZ->setEnabled(flag);
    ui->sB_figRotateX->setEnabled(flag);
    ui->sB_figScaleY->setEnabled(flag);
    ui->sB_figScaleZ->setEnabled(flag);
}
