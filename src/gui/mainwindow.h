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

private slots:
    void exit_show();
    void on_pushButton_sph_color_clicked();
    void error_message(QString str);
    void show_color(QColor color, QLabel* lab);

private:
    Ui::MainWindow* ui;
    QColor tmp;
};
#endif // MAINWINDOW_H