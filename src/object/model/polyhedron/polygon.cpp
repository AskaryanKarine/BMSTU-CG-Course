#include "polygon.h"

#include <QtGlobal>

QVector3D calcMP(const std::vector<QVector3D>& points)
{
    QVector3D MP(0, 0, 0);
    for (size_t i = 0; i < points.size(); i++)
        MP += points[i];

    MP /= points.size();
    return MP;
}

Polygon::Polygon()
{
}

Polygon::Polygon(const std::vector<size_t>& ver, const std::vector<QVector3D>& points)
    : _vertices(ver)
{
    calcNorm(points);
}

std::tuple<bool, double, QVector3D> Polygon::rayIntersect(const QVector3D& origin, const QVector3D& direction, const std::vector<QVector3D>& points)
{
    auto dir = direction.normalized();
    QVector3D n(0, 0, 0);
    auto r = std::tuple<bool, double, QVector3D>(false, 0, n);

    if (_vertices.size() < 3)
        return r;

    double k = _a * dir.x() + _b * dir.y() + _c * dir.z();
    if (qFuzzyIsNull(k))
        return r;

    double t = -(_a * origin.x() + _b * origin.y() + _c * origin.z() + _d) / k;
    if (t < 0)
        return r;

    QVector3D hit = origin + dir * t;
    if (!isInside(hit, points))
        return r;
    n = QVector3D(_a, _b, _c).normalized();
    return std::tuple<bool, double, QVector3D>(true, t, n);
}

void Polygon::calcNorm(const std::vector<QVector3D>& points)
{
    QVector3D mp = calcMP(points);
    QVector3D p0 = points[_vertices[0]];
    QVector3D p1 = points[_vertices[1]];
    QVector3D p2 = points[_vertices[2]];

    double kx = (p1.y() - p0.y()) * (p2.z() - p0.z()) - (p2.y() - p0.y()) * (p1.z() - p0.z());
    double ky = (p1.x() - p0.x()) * (p2.z() - p0.z()) - (p2.x() - p0.x()) * (p1.z() - p0.z());
    double kz = (p1.x() - p0.x()) * (p2.y() - p0.y()) - (p2.x() - p0.x()) * (p1.y() - p0.y());

    _a = kx;
    _b = -ky;
    _c = kz;
    _d = -kx * p0.x() + ky * p0.y() - kz * p0.z();

    if (_a * mp.x() + _b * mp.y() + _c * mp.z() + _d > 0.0) {
        _a = -_a;
        _b = -_b;
        _c = -_c;
        _d = -_d;
    }
}

inline int sign(int n)
{
    if (n > 0)
        return 1;
    if (n < 0)
        return -1;
    return 0;
}

bool Polygon::isInside(QVector3D& point, std::vector<QVector3D> points)
{
    int signX = 0;
    int signY = 0;
    int signZ = 0;

    for (size_t i = 0; i < _vertices.size(); i++) {
        QVector3D p0 = points[_vertices[i]];
        QVector3D p1 = points[_vertices[(i + 1) % _vertices.size()]];

        QVector3D side = p1 - p0;
        QVector3D toPoint = point - p0;

        QVector3D prod = QVector3D::crossProduct(side, toPoint);

        int curSignX = sign(prod.x());
        int curSignY = sign(prod.y());
        int curSignZ = sign(prod.z());

        if (curSignX != 0) {
            if (curSignX != signX && signX != 0)
                return false;
            signX = curSignX;
        }

        if (curSignY != 0) {
            if (curSignY != signY && signY != 0)
                return false;
            signY = curSignY;
        }

        if (curSignZ != 0) {
            if (curSignZ != signZ && signZ != 0)
                return false;
            signZ = curSignZ;
        }
    }

    return true;
}
