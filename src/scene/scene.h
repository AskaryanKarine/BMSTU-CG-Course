#ifndef SCENE_H
#define SCENE_H

#include "baselight.h"
#include "camera.h"
#include "model.h"

class Scene {
public:
    Scene();
    void init_scene();

private:
    std::vector<Model> _figures;
    std::vector<BaseLight> _lights;
    Camera _cam;
};

#endif // SCENE_H
