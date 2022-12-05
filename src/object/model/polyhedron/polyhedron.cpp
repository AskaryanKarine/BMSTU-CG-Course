#include "polyhedron.h"

Polyhedron::Polyhedron()
{
}

void Polyhedron::transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate)
{
}

std::tuple<bool, double, QVector3D> Polyhedron::rayIntersect(const QVector3D& origin, const QVector3D& direction)
{
    double t = 0.0;
    QVector3D norm(0, 0, 0);
    std::tuple<bool, double, QVector3D> r(false, t, norm);
    return r;
}
