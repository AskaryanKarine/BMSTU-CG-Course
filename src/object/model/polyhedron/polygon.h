#ifndef POLYGON_H
#define POLYGON_H

#include <QVector3D>

class Polygon {
public:
    Polygon();
    Polygon(const std::vector<size_t>& ver, const std::vector<QVector3D>& points);

    std::tuple<bool, double, QVector3D> rayIntersect(const QVector3D& origin, const QVector3D& direction, const std::vector<QVector3D>& points);
    void calcNorm(const std::vector<QVector3D>& points);
    bool isInside(QVector3D& point, std::vector<QVector3D> points);

private:
    std::vector<size_t> _vertices;
    double _a, _b, _c, _d;
};

#endif // POLYGON_H
