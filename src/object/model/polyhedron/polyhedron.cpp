#include "polyhedron.h"

Polyhedron::Polyhedron()
{
}

Polyhedron::Polyhedron(const std::vector<QVector3D>& points, const std::vector<Polygon>& polygins, const Material& m)
    : Model(m)
    , _points(points)
    , _pols(polygins)
{
    calcCenter();
}

void Polyhedron::transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate)
{
}

bool fuzzyZero(double n)
{
    return n > -1e-3 && n < 1e-3;
}

std::tuple<bool, double, QVector3D> Polyhedron::rayIntersect(const QVector3D& origin, const QVector3D& direction)
{
    double t = std::numeric_limits<double>::max();
    QVector3D norm(0, 0, 0);
    bool intersect = false;
    for (size_t i = 0; i < _pols.size(); i++) {
        auto intersectRes = _pols[i].rayIntersect(origin, direction, _points);
        double t1 = std::get<1>(intersectRes);
        if (t1 < t && fuzzyZero(t1)) {
            t = t1;
            norm = std::get<2>(intersectRes);
            intersect = true;
        }
    }
    return std::tuple<bool, double, QVector3D>(intersect, t, norm);
}

std::vector<QVector3D> Polyhedron::get_points()
{
    return _points;
}

void Polyhedron::set_points(std::vector<QVector3D> p)
{
    _points = p;
}

void Polyhedron::set_polygons(std::vector<Polygon> p)
{
    _pols = p;
}

void Polyhedron::calcCenter()
{
    _center = QVector3D(0, 0, 0);
    for (size_t i = 0; i < _points.size(); i++)
        _center += _points[i];

    _center /= _points.size();
}
