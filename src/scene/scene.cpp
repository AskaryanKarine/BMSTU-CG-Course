#include "scene.h"

#include "lightsource.h"
#include "material.h"
#include "objloader.h"
#include "polyhedron.h"
#include "sphere.h"

#include <QResource>
#include <iostream>

Scene::Scene()
{
}

void Scene::init_scene()
{
    _backgroungColor = Qt::black;
    // Элементы по умолчанию. 0 - главная сфера, 1 - нижняя плоскость, ниже которой нельзя поместить объекты
    // главная сфера
    Material m(1, 100, QVector4D(0.2, 0.9, 0.7, 0.5), QColor(188, 21, 79));
    auto mainSph = std::make_shared<Sphere>(QVector3D(480, 311, 100), 200, m);
    _obj.push_back(mainSph);

    // плоскость
    Material m1(1, 100, QVector4D(0.6, 1, 0, 0), QColor(105, 105, 0));
    auto Sph = std::make_shared<Sphere>(QVector3D(100, 311, -400), 10, m1);
    _obj.push_back(Sph);

    //    QResource plane_res(":/obj/plane.obj");
    //    auto r = plane_res.fileName();
    //    std::cout << r.toStdString() << std::endl;
    load_models("../src/obj/2.obj");

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

bool Scene::load_models(std::string filename)
{
    objl::Loader loader;

    bool loaded = loader.LoadFile(filename);

    if (loaded) {
        size_t minVertPos = 0;
        size_t minPolPos = 0;

        for (size_t i = 0; i < loader.LoadedMeshes.size(); i++) {
            objl::Mesh curMesh = loader.LoadedMeshes[i];

            Polyhedron p = Polyhedron();
            std::vector<QVector3D> points;
            std::vector<Polygon> polygons;

            for (size_t j = minVertPos; j < curMesh.Positions.size(); j++) {
                QVector3D point = QVector3D(curMesh.Positions[j].X, curMesh.Positions[j].Y, curMesh.Positions[j].Z);
                points.push_back(point);
            }

            minVertPos = curMesh.Positions.size();

            for (size_t j = minPolPos; j < curMesh.Indexes.size(); j += 3) {
                Polygon pol = Polygon({ curMesh.Indexes[j] - 1, curMesh.Indexes[j + 1] - 1, curMesh.Indexes[j + 2] - 1 }, points);
                polygons.push_back(pol);
            }

            minPolPos = curMesh.Indexes.size();

            p.set_points(points);
            p.set_polygons(polygons);
            p.set_material(Material(1, 100, QVector4D(0.6, 1, 0, 0), QColor(105, 105, 0)));

            _obj.push_back(std::make_shared<Polyhedron>(p));
        }
        return true;
    }
    return false;
}
