#ifndef PICTURE_H
#define PICTURE_H

#include "baselight.h"
#include "camera.h"
#include "model.h"
#include <QImage>
#include <QVector3D>
#include <memory>

class Picture {
public:
    Picture();
    Picture(int height, int width, int depth);

    void set_height(int height);
    void set_width(int width);

    void set_maxDepth(int max_depth);

    void set_backColor(QColor c);
    QColor get_backColor();

    void set_mainSphereColor(QColor c);
    QColor get_mainSphereColor();

    void set_refIndex(double r);
    void set_transparient(double tr);

    void move_camera(QVector3D offset);
    QVector3D get_cam_pos();

    void add_model();

    std::shared_ptr<QImage> drawingFigure();
    std::shared_ptr<QImage> drawingFigure(int nThr);
    void drawThr(int start, int end, std::shared_ptr<QImage> &img);

private:
    int _height;
    int _width;
    int _maxDepth;

    Camera _cam;
    QVector3D _screen;
    std::vector<std::shared_ptr<Model>> _obj;
    std::vector<std::shared_ptr<Model>> _primitives;
    std::vector<std::shared_ptr<BaseLight>> _lights;

    QColor _backgroungColor;

    void init();
    std::tuple<bool, double, int, QVector3D> scene_intersect(const QVector3D& orig, const QVector3D& dir);
    QColor cast_ray(const QVector3D& orig, const QVector3D& dir, int depth);
    void load_primitives(std::string filename);
};

#endif // PICTURE_H
