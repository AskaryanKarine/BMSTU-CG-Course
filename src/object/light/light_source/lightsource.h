#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "baselight.h"

class LightSource : public BaseLight
{
public:
    LightSource();
    ~LightSource() {};
    explicit LightSource(QVector3D pos);

    void transform() override; // TODO

};

#endif // LIGHTSOURCE_H
