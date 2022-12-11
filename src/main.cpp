#include "mainwindow.h"
#include "picture.h"

#include <QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc == 1) {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }

    Picture pic(672, 1230, 15);
    pic.added_light();
    pic.added_model(0, QColor(255, 69, 0));
    double i = 0.0;
    int n = 0;
    while (i - 1.0 < 1e3) {
        pic.set_transparient(i);
        auto img = pic.drawingFigure(16);
        std::cout << i << std::endl;
        QString name = QString::number(n) + ".png";
        img->save(name);
        n++;
        i += 0.01;
    }
}
