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

    std::shared_ptr<QImage> drawingFigure();
    std::shared_ptr<QImage> drawingFigure(int nThr);
    void drawThr(int start, int end, std::shared_ptr<QImage>& img);

    void added_model(int type_index, QColor color);
    void remove_model(int id);
    void transform_model(int id, QVector3D move, QVector3D scale, QVector3D rotate);
    QVector3D get_pos_model(int id);

    void added_light();
    void remove_light(int id);
    void trasform_light(int id, QVector3D move);
    QVector3D get_pos_light(int id);

    int get_count_light();
    int get_count_models();

    void change_fig_color(int id, QColor fc);

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
