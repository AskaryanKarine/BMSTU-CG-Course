#ifndef SCENE_H
#define SCENE_H

#include "baselight.h"
#include "camera.h"
#include "model.h"

class Scene {
public:
    Scene();
    void init_scene();

    std::vector<Model> get_model();
    void add_model(Model m);
    void remove_model(int id);
    void transform_model(int id, QVector3D m, QVector3D s, QVector3D r);

    std::vector<BaseLight> get_light();
    void add_light(BaseLight l);
    void remove_light(int id);
    void transform_light(int id, QVector3D m);

private:
    std::vector<Model> _obj;
    std::vector<BaseLight> _lights;
    Camera _cam;
};

#endif // SCENE_H
