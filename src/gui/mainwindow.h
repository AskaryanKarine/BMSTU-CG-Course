#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

#include "picture.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void showEvent(QShowEvent* ev);
    void resizeEvent(QResizeEvent* event);

private slots:
    void app_exit();
    void show_error(QString str);
    void set_color(QColor color, QLabel* lab);
    void windowShown();
    void on_pB_sphereColor_clicked();
    void on_pB_figColor_clicked();
    void on_pB_backgroundColor_clicked();
    void on_pB_draw_clicked();
    void on_pB_cameraLeft_clicked();
    void on_pB_cameraZoomPlus_clicked();
    void on_pB_cameraUp_clicked();
    void on_pB_cameraDown_clicked();
    void on_pB_cameraRight_clicked();
    void on_pB_cameraZoomMinus_clicked();
    void on_pB_figAdd_clicked();
    void on_pB_lightAdd_clicked();
    void on_pB_lightDel_clicked();
    void on_pB_figDel_clicked();

    void on_pB_monig_all_clicked();

    void on_cB_figAll_currentIndexChanged(int index);

private:
    Ui::MainWindow* ui;
    QColor figColor = Qt::black;
    Picture picture;
    std::shared_ptr<QImage> img;
    int camera_step;

    void color_dialog(QColor& color);
};
#endif // MAINWINDOW_H
