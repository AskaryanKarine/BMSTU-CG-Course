#include "baselight.h"

BaseLight::BaseLight(QVector3D pos, double i) : _position(pos), _intensity(i)
{

}

QVector3D BaseLight::get_position()
{
    return _position;
}

void BaseLight::set_position(QVector3D pos)
{
    _position = pos;
}

double BaseLight::get_intensity()
{
    return _intensity;
}
