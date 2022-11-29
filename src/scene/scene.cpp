#include "scene.h"

#include "material.h"
#include "sphere.h"

Scene::Scene()
{
}

void Scene::init_scene()
{
    _backgroungColor = Qt::black;
    Material m(1, 100, QVector4D(0.6, 1, 1, 0), QColor(200, 200, 200));
    auto mainSph = std::make_shared<Sphere>(QVector3D(550, 700, 100), 200, m);
    _obj.push_back(mainSph);
}

std::vector<std::shared_ptr<Model>> Scene::get_model()
{
    return _obj;
}

void Scene::add_model(std::shared_ptr<Model> m)
{
    _obj.push_back(m);
}

void Scene::remove_model(int id)
{
    _obj.erase(_obj.begin() + id);
}

void Scene::transform_model(int id, QVector3D m, QVector3D s, QVector3D r)
{
    _obj[id]->transform(m, s, r);
}

std::vector<std::shared_ptr<BaseLight>> Scene::get_light()
{
    return _lights;
}

void Scene::add_light(std::shared_ptr<BaseLight> l)
{
    _lights.push_back(l);
}

void Scene::remove_light(int id)
{
    _lights.erase(_lights.begin() + id);
}

void Scene::transform_light(int id, QVector3D m)
{
    _lights[id]->transform(m, QVector3D(1, 1, 1), QVector3D(0, 0, 0));
}

QColor Scene::get_backgroundColor()
{
    return _backgroungColor;
}

void Scene::set_backfroundColor(QColor bc)
{
    _backgroungColor = bc;
}

// Camera Scene::get_camera()
//{
//     return _cam;
// }

// void Scene::set_camera(Camera c)
//{
//     _cam = c;
// }
