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
    _normal = QVector3D(0, 0, 0);
    QVector3D L = _center - origin; // соед. центр сферы и начало луча
    double tca = QVector3D::dotProduct(L, direction); // проекция L на луч (скаляр.произв.)
    double d2 = QVector3D::dotProduct(L, L) - tca * tca; // ближайшая к центру сферы точка луча (ее сдвиг от начала,
    // как сторона прямоуг. треугольника)

    double rad2 = _rad * _rad;
    if (d2 > rad2) // пересечение вне сферы, не пересекает типа
        return false;

    double thc = sqrt(rad2 - d2); // расстояние от той ближайшей точки до пересечения

    double t0 = tca - thc;
    double t1 = tca + thc; // 0 - если внутри сферы, 1 - если снаружи
    if (t0 > 1.0)
        t = t0;
    else if (t1 > 1.0)
        t = t1;
    else
        return false;

    QVector3D pt = origin + direction * t;
    _normal = (pt + (_center * (-1))).normalized();
    return true;
}

void Sphere::transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate)
{
    _center += move;
    _rad *= scale[0];
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
