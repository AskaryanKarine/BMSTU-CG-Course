#include "lightsource.h"

LightSource::LightSource()
{
}

LightSource::LightSource(QVector3D pos)
    : BaseLight(pos)
{
}

void LightSource::transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate)
{
    _position += move;
}
