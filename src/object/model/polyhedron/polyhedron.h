#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include "model.h"
#include "polygon.h"

class Polyhedron : public Model {
public:
    Polyhedron();
    ~Polyhedron() {};

    void transform(const QVector3D& move, const QVector3D& scale, const QVector3D& rotate) override;
    std::tuple<bool, double, QVector3D> rayIntersect(const QVector3D& origin, const QVector3D& direction) override;

private:
    QVector3D _center;
    std::vector<QVector3D> _points;
    std::vector<Polygon> _pols;
};

#endif // POLYHEDRON_H
