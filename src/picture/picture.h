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

    //    void set_object

    //    QVector3D cast_ray();

    std::shared_ptr<QImage> drawingFgure();

    std::shared_ptr<QImage> drawingFigure(int nThr);
    void drawThr(int start, int end, std::shared_ptr<QImage>& img);

private:
    int _height;
    int _width;
    int _maxDepth;

    Scene _scene;
};

#endif // PICTURE_H
