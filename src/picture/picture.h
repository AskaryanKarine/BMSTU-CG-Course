#ifndef PICTURE_H
#define PICTURE_H

#include "scene.h"

#include <QImage>
#include <QVector3D>
#include <memory>

class Picture {
public:
    Picture();
    Picture(int height, int width, int depth);

    void set_height(int height);
    int get_height();

    void set_width(int width);
    int get_width();

    void set_maxDepth(int max_depth);
    int get_maxDepth();

    void set_backColor(QColor c);
    QColor get_backColor();

    void set_mainSphereColor(QColor c);
    QColor get_mainSphereColor();

    void set_refIndex(double r);

    QColor cast_ray(QVector3D orig, QVector3D dir, int depth);
    std::shared_ptr<QImage> drawingFgure();
    std::shared_ptr<QImage> drawingFigure(int nThr);
    void drawThr(int start, int end, std::shared_ptr<QImage>& img);

private:
    int _height;
    int _width;
    int _maxDepth;

    Scene _scene;
    Camera _cam;

    bool scene_intersect(QVector3D orig, QVector3D dir, QVector3D& hit, QVector3D& N, Material& m, int& closest);
    void init();
};

#endif // PICTURE_H
