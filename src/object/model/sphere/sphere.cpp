#include "sphere.h"

Sphere::Sphere()
{
}

Sphere::Sphere(const QVector3D& center, const double& radius, const Material& m)
    : Model(m)
    , _rad(radius)
    , _center(center)

{
}

bool Sphere::rayIntersect(const QVector3D& origin, const QVector3D& direction, double& t)
{
    return true;
}

void Sphere::transform()
{

}

double Sphere::get_radius()
{
    return _rad;
}

void Sphere::set_radius(double r)
{
    _rad = r;
}

QVector3D Sphere::get_center()
{
    return _center;
}

void Sphere::set_center(QVector3D center)
{
    _center = center;
}
