#include "scene.h"

#include "lightsource.h"
#include "material.h"
#include "sphere.h"

#include <iostream>

Scene::Scene()
{
}

void Scene::init_scene()
{
    _backgroungColor = Qt::black;
    // Элементы по умолчанию. 0 - главная сфера, 1 - нижняя плоскость, ниже которой нельзя поместить объекты
    Material m(1, 100, QVector4D(0.2, 0.9, 0.7, 0.5), QColor(188, 21, 79));
    auto mainSph = std::make_shared<Sphere>(QVector3D(480, 311, 100), 200, m);
    _obj.push_back(mainSph);

    Material m1(1, 100, QVector4D(0.6, 1, 0, 0), QColor(105, 105, 0));
    auto Sph = std::make_shared<Sphere>(QVector3D(300, 311, -400), 200, m1);
    _obj.push_back(Sph);

    auto l = std::make_shared<LightSource>(QVector3D(0, 0, 0));
    _lights.push_back(l);
    auto l1 = std::make_shared<LightSource>(QVector3D(700, 500, 200));
    _lights.push_back(l1);
    auto l2 = std::make_shared<LightSource>(QVector3D(500, 500, 500));
    _lights.push_back(l2);
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
    for (auto object : _obj) {
        auto m = object->get_material();
        m.set_specColor(bc);
        object->set_material(m);
    }
}

QColor Scene::get_mainSphereColor()
{
    return _obj[0]->get_material().get_difColor();
}

void Scene::set_mainSphereColor(QColor c)
{
    auto m = _obj[0]->get_material();
    m.set_difColor(c);
    _obj[0]->set_material(m);
}

void Scene::set_refractiveIndex(double r)
{
    auto m = _obj[0]->get_material();
    m.set_refractive_index(r);
    _obj[0]->set_material(m);
}

void Scene::set_trasparient(double r)
{
    auto m = _obj[0]->get_material();
    QVector4D tr = m.get_albedo();
    tr.setW(r);
    m.set_albedo(tr);
    _obj[0]->set_material(m);
}
