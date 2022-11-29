#ifndef SCENE_H
#define SCENE_H

#include "baselight.h"
#include "camera.h"
#include "model.h"
#include <QColor>

class Scene {
public:
    Scene();
    void init_scene();

    std::vector<std::shared_ptr<Model>> get_model();
    void add_model(std::shared_ptr<Model> m);
    void remove_model(int id);
    void transform_model(int id, QVector3D m, QVector3D s, QVector3D r);

    std::vector<std::shared_ptr<BaseLight>> get_light();
    void add_light(std::shared_ptr<BaseLight> l);
    void remove_light(int id);
    void transform_light(int id, QVector3D m);

    QColor get_backgroundColor();
    void set_backfroundColor(QColor bc);

    // получить и установить цвет линзы

private:
    std::vector<std::shared_ptr<Model>> _obj;
    std::vector<std::shared_ptr<BaseLight>> _lights;

    QColor _backgroungColor;
};

#endif // SCENE_H