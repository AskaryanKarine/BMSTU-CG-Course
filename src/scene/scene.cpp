#include "scene.h"

#include "sphere.h"

Scene::Scene()
{
}

void Scene::init_scene()
{
    _cam.set_position(QVector3D(0, 0, 3000));
    //    _obj.push_back(Sphere());
}

// std::vector<Model> Scene::get_model()
//{
////    return _obj;
//}

// void Scene::add_model(Model m)
//{
//     //    _obj.push_back(m);
// }

void Scene::remove_model(int id)
{
    _obj.erase(_obj.begin() + id);
}

void Scene::transform_model(int id, QVector3D m, QVector3D s, QVector3D r)
{
    _obj[id].transform(m, s, r);
}

// std::vector<BaseLight> Scene::get_light()
//{
//     return _lights;
// }

// void Scene::add_light(BaseLight l)
//{
//     //    _lights.push_back(l);
// }

void Scene::remove_light(int id)
{
    _lights.erase(_lights.begin() + id);
}

void Scene::transform_light(int id, QVector3D m)
{
    _lights[id].transform(m, QVector3D(1, 1, 1), QVector3D(0, 0, 0));
}
