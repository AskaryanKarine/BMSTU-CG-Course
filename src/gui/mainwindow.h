#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>

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

private slots:
    void app_exit();
    void show_error(QString str);
    void set_color(QColor color, QLabel* lab);
    void windowShown();
    void on_pB_sphereColor_clicked();

private:
    Ui::MainWindow* ui;
    QColor tmp = Qt::black;
};
#endif // MAINWINDOW_H
