#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "object.h"
#include <QVector3D>

class BaseLight : public Object {
public:
    BaseLight() = default;
    explicit BaseLight(QVector3D pos, double i = 1);
    virtual ~BaseLight() = default;

    QVector3D get_position();
    void set_position(QVector3D pos);

    double get_intensity();
    void set_intensity(double i);

protected:
    QVector3D _position;
    double _intensity;
};

#endif // BASELIGHT_H
