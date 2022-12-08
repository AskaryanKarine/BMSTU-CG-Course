#include "polyhedron.h"

#include <iostream>

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

inline double toRad(double angle)
{
    return angle * M_PI / 180.0;
}

void rotate(double& x, double& y, const double& a)
{
    double rad = toRad(a);
    double tX = x;
    double tY = y;

    x = tX * cos(rad) + tY * sin(rad);
    y = -tX * sin(rad) + tY * cos(rad);
}

void point_rotate(QVector3D& point, QVector3D angle)
{
    double x = point.x();
    double y = point.y();
    double z = point.z();

    rotate(x, y, angle.z());
    rotate(x, z, angle.y());
    rotate(y, z, angle.x());

    point.setX(x);
    point.setY(y);
    point.setZ(z);
}

void Polyhedron::transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate)
{
    _move(move);
    calcCenter();
    _scale(scale);
    _rotate(rotate);

    for (size_t i = 0; i < _points.size(); i++)
        std::cout << "v " << _points[i].x() << " " << _points[i].y() << " " << _points[i].z() << std::endl;
    std::cout << "-----------" << std::endl;
}

void Polyhedron::_rotate(const QVector3D& rotate)
{
    _move(QVector3D(-_center.x(), -_center.y(), -_center.z()));
    for (auto& p : _points)
        point_rotate(p, rotate);
    _move(QVector3D(_center.x(), _center.y(), _center.z()));
    for (auto& p : _pols)
        p.calcNorm(_points);
}

void Polyhedron::_move(const QVector3D& move)
{
    for (auto& p : _points)
        p += move;

    for (auto& p : _pols)
        p.calcNorm(_points);
}

void Polyhedron::_scale(const QVector3D& scale)
{
    for (auto& p : _points)
        p *= scale;

    for (auto& p : _pols)
        p.calcNorm(_points);
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
        if (std::get<0>(intersectRes)) {
            double t1 = std::get<1>(intersectRes);
            if (t1 < t && !fuzzyZero(t1)) {
                t = t1;
                norm = std::get<2>(intersectRes);
                intersect = true;
            }
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
    calcCenter();
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
