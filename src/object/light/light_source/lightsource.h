#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "baselight.h"

class LightSource : public BaseLight {
public:
    LightSource();
    ~LightSource() {};
    explicit LightSource(QVector3D pos);

    void transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate) override; // TODO
};

#endif // LIGHTSOURCE_H
